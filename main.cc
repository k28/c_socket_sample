#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <strings.h>

int main() {

    std::string dest_ip = "127.0.0.1";
    int port = 8080;
    unsigned char send_data[] = {0x00};
    unsigned char reiceve_data[1024];

    int sockfd = -1;
    do {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("socket error.\n");
            break;
        }

        // 接続先のアドレス設定
        struct sockaddr_in servaddr;
        bzero(&servaddr, sizeof(sockaddr_in));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(dest_ip.c_str());

        // socketのタイムアウト設定
        struct timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 500;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            // 接続エラー
            printf("connect error.\n");
            break;
        }

        // 接続が確立したので、通信処理
        int sent_size = 0;
        int total_size = sizeof(send_data);
        unsigned char *send_ptr = send_data;
        while (sent_size < total_size) {
            int s = write(sockfd, send_ptr, total_size - sent_size);
            send_ptr += s;
            sent_size += s;
        }

        // 受信データ受信
        int n = 0;
        while ((n = read(sockfd, reiceve_data, sizeof(reiceve_data)) > 0 )) {
            reiceve_data[n] = 0;    // NULL

            printf("## %s\n", reiceve_data);
        }

    } while(0);

    // socketを開いていればクローズ
    if (sockfd < 0) {
        close(sockfd);
    }


    return 0;
}


