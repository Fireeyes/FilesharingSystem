typedef enum {REQUEST, DATA} packet_type;

typedef struct packet {
    int length;
    packet_type type;
    char* data;
} packet_t;
