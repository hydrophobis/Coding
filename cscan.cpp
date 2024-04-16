#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define PACKET_SIZE     64
#define TIMEOUT_SEC     1     // Timeout for socket operations (in seconds)

// Function to calculate checksum for ICMP packet
unsigned short checksum(unsigned short *ptr, int nbytes) {
    unsigned long sum;
    unsigned short oddbyte;
    unsigned short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char *)&oddbyte) = *(unsigned char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (unsigned short)~sum;
    return answer;
}

// Function to send ICMP Echo Request packet
void sendPing(const char *target_ip) {
    int sockfd;
    struct sockaddr_in target_addr;
    char packet[PACKET_SIZE];

    // Create raw socket
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("socket() failed");
        return;
    }

    // Prepare ICMP packet
    struct icmphdr *icmp_header = (struct icmphdr *)packet;
    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0;
    icmp_header->checksum = 0;
    icmp_header->un.echo.id = getpid();
    icmp_header->un.echo.sequence = 0;
    memset(packet + sizeof(struct icmphdr), 'A', PACKET_SIZE - sizeof(struct icmphdr));
    icmp_header->checksum = checksum((unsigned short *)packet, sizeof(packet));

    // Set target address
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(target_ip);

    // Send packet
    if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0) {
        perror("sendto() failed");
        close(sockfd);
        return;
    }

    close(sockfd);
}

// Function to list files in a directory
void listFiles(const char *dir_path, std::ostream& output) {
    struct dirent *entry;
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir() failed");
        return;
    }

    output << "Files in directory " << dir_path << ":" << std::endl;
    while ((entry = readdir(dir)) != NULL) {
        output << "- " << entry->d_name << std::endl;
    }

    closedir(dir);
}

// Function to read a file
void readFile(const char *file_path, std::ostream& output) {
    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("open() failed");
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        output.write(buffer, bytes_read);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 1 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [-o <filename>]" << std::endl;
        return EXIT_FAILURE;
    }

    const char *target_ip = argv[1];
    std::ostream *output_stream = &std::cout; // Default to stdout

    if (argc == 4 && strcmp(argv[2], "-o") == 0) {
        // Output redirection requested
        const char *filename = argv[3];
        std::ofstream file_output(filename);
        if (!file_output) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return EXIT_FAILURE;
        }
        output_stream = &file_output;
    }

    // Call the function to send ICMP Echo Request packet
    sendPing(target_ip);

    // Gather information about the system
    *output_stream << "System Information:" << std::endl;
    system("uname -a");
    system("lsb_release -a");

    // List files in the current directory
    listFiles(".", *output_stream);

    // Read contents of /etc/passwd file
    *output_stream << "Contents of /etc/passwd:" << std::endl;
    readFile("/etc/passwd", *output_stream);

    return EXIT_SUCCESS;
}
