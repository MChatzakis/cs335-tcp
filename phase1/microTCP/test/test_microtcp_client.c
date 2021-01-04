/*
 * microtcp, a lightweight implementation of TCP for teaching,
 * and academic purposes.
 *
 * Copyright (C) 2015-2017  Manolis Surligas <surligas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * You can use this file to write a test microTCP client.
 * This file is already inserted at the build system.
 */

/**
 * @file test_microtcp_client.c
 * @author Manos Chatzakis (chatzakis@ics.forth.gr)
 * @author George Kokolakis (gkokol@ics.forth.gr)
 * @brief An example of micro tcp implementation following client-server model.
 * @version 0.1
 * @date 2020-11-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../lib/microtcp.h"
#include "../utils/crc32.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/crc32.h"

#define MANY_DATA 1000000

#define MAXSIZE 100
#define PORT 8080

void generateBytes(size_t N, char *buff)
{
    int i;
    //buff = malloc(sizeof(char) * N);
    for (i = 0; i < N; i++)
    {
        buff[i] = 'c';
    }
}

int main(int argc, char **argv)
{
    int len, n;

    //char *str1 = "Client 1!";
    char str1[MANY_DATA];
    

    char *str2 = "Client 2!";
    char *str3 = "Client 3!";

    char buffer[MAXSIZE];

    generateBytes(MANY_DATA,str1);

    microtcp_sock_t socket;
    struct sockaddr_in servaddr;

    printf("Client running...\n");

    //printf("%s -> %x\n",str1,crc32(str1,9));

    socket = microtcp_socket(AF_INET, SOCK_DGRAM, 0);
    if (socket.state == INVALID)
    {
        perror("Microtcp socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    /*Filling server information*/
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if ((microtcp_connect(&socket, (const struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
    {
        perror("Connect\n");
        exit(EXIT_FAILURE);
    }

    n = microtcp_send(&socket, (const char *)str1, MANY_DATA, 0);
    if (n != MANY_DATA)
    {
        perror("Could not send data\n");
        exit(EXIT_FAILURE);
    }
    printf("Send message to server of %d bytes.\n", n);
   /* int x = 3;
    int * ptr = &x;
    //*ptr = 3;
    n = microtcp_send(&socket, (const int *)ptr, 4, 0);
    if (n != 4)
    {
        perror("Could not send data\n");
        exit(EXIT_FAILURE);
    }
    printf("Send message to server of %d bytes.\n", n);

    /*n = microtcp_send(&socket, (const char *)str3, 10, 0);
    if (n != 10)
    {
        perror("Could not send data\n");
        exit(EXIT_FAILURE);
    }

    printf("Send message to server of %d bytes.\n", n);
*/
    if (microtcp_shutdown(&socket, 0) < 0)
    {
        perror("Shut down\n");
        exit(EXIT_FAILURE);
    }

    if (socket.state == CLOSED)
    {
        printf("Shutdown succeed\n");
    }

    return 0;
}
