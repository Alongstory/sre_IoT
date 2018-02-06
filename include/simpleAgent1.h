void print_usage();
int error(int err, const char *format, ...);
void parse_options(int argc, char* argv[]);
void sendConfirmationCbk(iothubAgent_messageId p_messageId, iothubAgent_Error p_err);
iothubAgent_Error messageReceivedCbk(iothubAgent_message* p_message);
int filereadline(const char* filename, char** pstr);
int filewrite(const char* filename, const char* ptr, size_t size);
const char* getConnectionStringFilename();
void loadConnectionString();
void saveConnectionString();
iothubAgent_Error start();
iothubAgent_Error sendMessage(const char* id, const char* payload, size_t size);

char* connectionString;
const char* deviceEndPointName;
const char* bootstrapKey;
char* proxy;
bool stop;
