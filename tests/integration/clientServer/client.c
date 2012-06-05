/* Client of a client-server application designed to check 
   that Comm module works correctly between different machines

   Syntax = client hostname port
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "comm.h"
#include "trains.h" // To have message typedef

#define CONNECT_TIMEOUT 2000 // milliseconds

#define HW "Hello world!"
#define LONG_MESSAGE "This is a long message: 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define VERY_LONG_MESSAGE_SIZE 1000000
#define NB_CHUNKS 4 //Number of chunks used to send message of VERY_LONG_MESSAGE_SIZE
#define IOVCNT (1 + NB_CHUNKS)

int main(int argc, char *argv[]) {
  t_comm *commForConnect;
  message *msg;
  int len, lenIncomplete, nbWritten;
  struct iovec iov[IOVCNT];
  int i;

  if (argc != 3){
    fprintf(stderr, "USAGE = %s hostname port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // We open a connection to send messages
  printf("Connecting %s on port %s...\n", argv[1], argv[2]);
  commForConnect = comm_newAndConnect(argv[1], argv[2], CONNECT_TIMEOUT);
  if (commForConnect == NULL)
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "comm_newAndConnect");
  printf("...OK\n");

  len = sizeof(message_header)+strlen(HW)+sizeof('\0');
  msg = malloc(len);
  assert(msg != NULL);
  msg->header.len = len;
  strcpy(msg->payload, HW);
  printf("\tSend message of %d bytes with: \"%s\"...\n", len, HW);
  nbWritten = comm_write(commForConnect, msg, msg->header.len);
  if (nbWritten != len){
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "sent only %d/%d bytes", nbWritten, len);
  }
  free(msg);

  len = sizeof(message_header)+strlen(LONG_MESSAGE)+sizeof('\0');
  msg = malloc(len);
  assert(msg != NULL);
  msg->header.len = len;
  strcpy(msg->payload, LONG_MESSAGE);
  printf("\tSend message of %d bytes with: \"%s\"...\n", len, LONG_MESSAGE);
  nbWritten = comm_write(commForConnect, msg, msg->header.len);
  if (nbWritten != len){
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "sent only %d/%d bytes", nbWritten, len);
  }
  free(msg);

  // We send a very long message
  len = sizeof(message_header)+VERY_LONG_MESSAGE_SIZE;
  msg = malloc(len);
  assert(msg != NULL);
  msg->header.len = len;
  for(i=0; i<VERY_LONG_MESSAGE_SIZE; i++){
    msg->payload[i] = i%256;
  }
  printf("\tSend message of %d bytes\n", len);
  iov[0].iov_base = msg;
  iov[0].iov_len  = sizeof(message_header);
  for(i=0; i<NB_CHUNKS; i++){
    iov[i+1].iov_base = &(msg->payload[i*VERY_LONG_MESSAGE_SIZE/NB_CHUNKS]);
    iov[i+1].iov_len  = VERY_LONG_MESSAGE_SIZE/NB_CHUNKS;
  }
  nbWritten = comm_writev(commForConnect, iov, IOVCNT);
  if (nbWritten != len){
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "sent only %d/%d bytes", nbWritten, len);
  }
  // We do not free msg yet, as we reuse it in the following lines
  //free(msg);

  // We send a very long message which is not complete
  lenIncomplete = len - VERY_LONG_MESSAGE_SIZE/NB_CHUNKS;
  printf("\tSend incomplete message of %d/%d bytes  ==> This should make the server unhappy\n", lenIncomplete, len);
  nbWritten = comm_writev(commForConnect, iov, IOVCNT-1);
  if (nbWritten != lenIncomplete){
    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "sent only %d/%d bytes", nbWritten, lenIncomplete);
  }
  free(msg);



  // We sleep a little to give time to the message to arrive before closing 
  // connection
  // NB : the usleep is specific to this integration test! It is not necessary 
  //      in a standard application.
  usleep(1000000);
  printf("Close connection...\n");
  comm_free(commForConnect);
  printf("...OK\n");

  return EXIT_SUCCESS;
}

  
