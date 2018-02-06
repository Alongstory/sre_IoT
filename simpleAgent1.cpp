#include "iothubAgent.h"

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string>
#include <assert.h>
#include <curl.h>




char* connectionString=NULL;
const char* deviceEndPointName=NULL;
const char* bootstrapKey=NULL;
char* proxy=NULL;
bool stop=false;

//function from curl
extern "C" size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
//=========================================================================
//
//=========================================================================
extern "C" int error(int err, const char *format, ...)
{
	flockfile(stdout);

	va_list list;
	va_start(list, format);
	printf("error %d : ", err);
	vprintf(format, list);
	va_end(list);
	printf("\n");
	funlockfile(stdout);
	
	return err;
}

//=========================================================================
//
//=========================================================================
extern "C" void print_usage() {
    printf("IOT Hub Device Simulator\n");
    printf("Usage: simpleAgent [options]\n");
    printf("\n");
    printf("-h help                Print usage, then exit\n");
    printf("-b BSK                 Set the bootstrap key with BSK\n");
    printf("-c CS                  Set the connection string with CS\n");
    printf("-d ID                  Set the device ID\n");
    printf("\n");
    printf("To make it start:\n");
    printf("    Bootstrap information must be provided (deiveId and bootstrapKey)\n");
    printf("    Or connection string must be provided (connectionString)\n");
    printf("\n");
}

//=========================================================================
//
//=========================================================================
extern "C" void parse_options(int argc, char* argv[]) {
    int opt;

    if(argc == 1) {
        print_usage();
        exit(EXIT_SUCCESS);
    }

    while ((opt = getopt(argc, argv, "hb:c:d:p:t:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
                break;
            case 'b':
            	bootstrapKey = optarg;
                break;
            case 'c':
                connectionString = optarg;
              break;
            case 'd':
            	deviceEndPointName = optarg;
                break;
            case 'p':
                proxy = optarg;
                break;
            default: /* '?' */
              printf("Unsupported command line parameter %s => EXIT\n", optarg);
              exit(EXIT_FAILURE);
        }
    }
}


//=========================================================================
//
//=========================================================================
extern "C" void sendConfirmationCbk(iothubAgent_messageId p_messageId, iothubAgent_Error p_err) 
{
    printf("Message [%s] successfully reached  IoT Hub with error %d\n", p_messageId.id, p_err);
}

//=========================================================================
//
//=========================================================================
extern "C" iothubAgent_Error messageReceivedCbk(iothubAgent_message* p_message)
{
    std::string payload( (char*)p_message->payload, p_message->size);
    printf("Incoming Message [%s] : %s (%zu bytes)\n", p_message->id.id, payload.data(), p_message->size);
    char *hint = "newlua";
    //to get lua url here
    char * output = strdup(payload.data());   
    char *pos = strstr(output, hint);  
    
    if (pos != NULL){
		char *luaAdp = strstr(pos, "http://");
		int len = strlen(luaAdp), i;
		char luaAd[100];
		for (i = 0; i < (len - 2); i++){
			luaAd[i] = luaAdp[i];
		}
		

		printf("the lua script located at address: %s\n", luaAd);
		printf("getting the descriptor...\n");
		pos = strstr(luaAd, "fn=");
		char lua_name[FILENAME_MAX];
		len = strlen(pos);
		for (i = 0; i < (len - 3) ; i++){
			lua_name[i] = pos[i+3];
		}
				
		strcat(lua_name, ".nonsigned");		
		printf("the name of the file is: %s\n", lua_name);
		
		//////////////////
		CURL *curl;
		FILE *fp;
		CURLcode res;
		char *url = luaAd;
			
		char outfilename[FILENAME_MAX] = "/home/root/SRE_Console/lua/mock_rules/";		
		strcat(outfilename, lua_name);
		printf("...saving it to path: %s\n", outfilename);
		
		curl = curl_easy_init();
		if (curl) {
			fp = fopen(outfilename,"wb");
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			/* always cleanup */
			curl_easy_cleanup(curl);
			fclose(fp);
		}
	}
    if (strncmp((const char*) p_message->payload,"quit",p_message->size)==0)
        stop=true;
    return IOTHUBAGENT_OK;
    
}


//=========================================================================
//
//=========================================================================
// Note that the return string is always empty if there is an error
extern "C" int filereadline(const char* filename, char** pstr)
{
	assert(*pstr==NULL);

	FILE* f = fopen(filename,"r");
	if (f==NULL) {
		return error(errno,"fopen(%s) failed", filename);
	}

	const char* fmt="%m[^\n]";
	int x = fscanf(f, fmt, pstr);
	if (ferror(f)) {
		fclose(f);
		return error(errno,"fread(%s) error", filename);
	} else if (x!=1) {
		fclose(f);
		return error(-1,"fread(%s) too much data",filename);
	}

	int y = fclose(f);
	if (y<0) {
		return error(errno,"fclose(%s) error",filename);
	}

	return 0;
}


//=========================================================================
//
//=========================================================================
extern "C" int filewrite(const char* filename, const char* ptr, size_t size)
{
	FILE* f = fopen(filename,"w");
	if (f==NULL)
		return error(errno,"fopen(%s) failed",filename);

	size_t x = fwrite(ptr, 1, size, f);
	if (ferror(f)) {
		fclose(f);
		return error(errno,"fwrite(%s) error",filename);
 	} else if (x!=size) {
		fclose(f);
		return error(-1,"fwrite(%s) data not written",filename);
	}

	int y = fclose(f);
	if (y<0)
		return error(errno,"fclose(%s) error",filename);

	return 0;
}

//=========================================================================
//
//=========================================================================
extern "C" const char* getConnectionStringFilename()
{
	assert(deviceEndPointName!=NULL);
	
	static char filename[255];
	snprintf(filename, sizeof(filename), "simpleAgent.connectionString.%s", deviceEndPointName);
	
	return filename;
}


//=========================================================================
//
//=========================================================================
extern "C" void loadConnectionString()
{
	assert(connectionString==NULL);
	
	const char* filename=getConnectionStringFilename();
	printf("Loading ConnectionString from %s\n",filename);
	int x = filereadline(filename, &connectionString );
	if (x!=0)
		error(-1,"Failed to load ConnectionString");
}


//=========================================================================
//
//=========================================================================
extern "C" void saveConnectionString()
{
	assert(connectionString!=NULL);

	const char* filename=getConnectionStringFilename();
	int x = filewrite(filename, connectionString, strlen(connectionString) );
	if (x!=0)
		error(-1,"Failed to save ConnectionString");

	printf("ConnectionString saved to %s\n",filename);
}


//=========================================================================
//
//=========================================================================
extern "C" iothubAgent_Error start()
{
    iothubAgent_Error err = IOTHUBAGENT_OK;

    if(connectionString == NULL) {
        if(deviceEndPointName == NULL) {
            err = IOTHUBAGENT_KO;
            printf("No device endpoint name set\n");
        } else {
			loadConnectionString();
		}
	}

    if(connectionString == NULL) {
        if(deviceEndPointName == NULL) {
        } else {
			printf("No saved ConnectionString, doing bootstrap\n");
            iothubAgent_bootstrapServiceCredentials bsCreds;

            bsCreds.deviceEndpointId = deviceEndPointName;
            bsCreds.bootstrapKey = bootstrapKey;
            //bsCreds.bootstrapServiceURL = "https://bootstrap.dev.struxurewarecloud.com";
            bsCreds.bootstrapServiceURL = "https://bootstrap2.preview.struxurewarecloud.com";
			char* cs=NULL;
            err = iothubAgent_bootstrap(&bsCreds, &cs);  /////////////////////////////////////////////////////////////
			connectionString=cs;
            if(err != IOTHUBAGENT_OK) {
              error(err, "iothubAgent_bootstrap");
            } else {
				printf("connectionString = %p %s\n", (void*)&connectionString, connectionString);
				saveConnectionString();
			}
        }
    } else {
        printf("Skip bootstrapping\n");
    }

    if(err == IOTHUBAGENT_OK) {
        err = iothubAgent_start(connectionString);
        if(err == IOTHUBAGENT_OK) {
            printf("iothubAgent starting\n");
        } else {
            error(err, "iothubAgent_start");
        }
    }

  return err;
}


//=========================================================================
//
//=========================================================================
extern "C" iothubAgent_Error sendMessage(const char* id, const char* payload, size_t size)
{
    iothubAgent_message message = { (const unsigned char*)payload, size, {id} };
    iothubAgent_Error err = iothubAgent_sendMessage(&message); 
    if (err!=IOTHUBAGENT_OK)
        error(err,"iothubAgent_sendMessage");
    return err;
}


//=========================================================================
//
//=========================================================================
/*int main(int argc, char* argv[])
{
    iothubAgent_Error err;

    //---------------------------------------------------------------------
    // Parse Args
    //---------------------------------------------------------------------
    parse_options(argc,argv);

    //---------------------------------------------------------------------
    // Init
    //---------------------------------------------------------------------
    iothubAgent_configuration conf = {
      sendConfirmationCbk,
      messageReceivedCbk,
      NULL,
      NULL,NULL,NULL


      };
    err = iothubAgent_init(&conf);
    if (err!=IOTHUBAGENT_OK)
        return error(err,"iothubAgent_init");

    //---------------------------------------------------------------------
    // Start
    //---------------------------------------------------------------------
    err = start();
    if (err!=IOTHUBAGENT_OK)
        return EXIT_FAILURE;

    //---------------------------------------------------------------------
    // Send
    //---------------------------------------------------------------------
    const char* message="VIc";
    err = sendMessage("Hi Victor", message, strlen(message));
    if (err!=IOTHUBAGENT_OK)
        return EXIT_FAILURE;

    //---------------------------------------------------------------------
    // Receive
    //---------------------------------------------------------------------
    while (!stop)
    {
        printf("waiting...\n");
        sleep(4);
    }

    //---------------------------------------------------------------------
    // Stop
    //---------------------------------------------------------------------
    err = iothubAgent_stop();
    if (err!=IOTHUBAGENT_OK)
        return error(err,"iothubAgent_stop");

    //---------------------------------------------------------------------
    // Shutdown
    //---------------------------------------------------------------------
    err = iothubAgent_shutdown();
    if (err!=IOTHUBAGENT_OK)
        return error(err,"iothubAgent_shutdown");


    return 0;
}
*/
