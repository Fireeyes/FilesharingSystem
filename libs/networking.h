typedef enum {REQUEST, DATA} packet_type;
typedef enum {ACTIVE, INACTIVE} connection_status;

typedef struct packet {
    int length; // just the length of the buffer
    packet_type type;
    char data[4096];
} packet_t;

typedef struct connection {
    connection_status status;
    int uid;    // user_id
    int socket; 
} connection_t;