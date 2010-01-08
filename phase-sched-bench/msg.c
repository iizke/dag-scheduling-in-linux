#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

#define MSG_TYPE 2

struct msgbuf {
    long int mtype;
    char mtext[1];
};

int main()
/*
 Sender: tao mot msgqueue, va gui mot msg len do
 */
{
    int key;
    int id;
    int i;
    struct msgbuf *pbuf;
    struct msqid_ds buf;
    char *text;

    /*
     Tao ra msgqueue
     */
    key = ftok("/", '0');
    id = msgget(key, IPC_CREAT|0666);
    printf("msgid = %d, key = %d \n", id, key);

    /*
     Gui mot doan text
     */
    text = "Hello ! MSG No.1";
    pbuf = (struct msgbuf*) malloc(sizeof(struct msgbuf) + strlen(text));
    pbuf->mtype = MSG_TYPE;
    strcpy(pbuf->mtext, text);

    msgsnd(id, pbuf, 10, 0);
    printf("Da gui doan text: Hello ! MSG No.1 \n");
    //	msgsnd(id, &pbuf, 10, IPC_NOWAIT);

    return 0;
}
