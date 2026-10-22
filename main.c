#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// E++ (E Plus Plus) Basic Lexer & Interpreter Prototype

void interpret_eplusplus(char* code) {
    // print কমান্ড চেক করার জন্য
    if (strncmp(code, "print", 5) == 0) {
        // কোডের ভেতর ডাবল কোট (" ") এর ভেতর কি আছে তা খোঁজা
        char* start = strchr(code, '"');
        if (start != NULL) {
            char* end = strchr(start + 1, '"');
            if (end != NULL) {
                int len = end - (start + 1);
                char message[256];
                strncpy(message, start + 1, len);
                message[len] = '\0';
                
                // ই প্লাস প্লাস ভাষার আউটপুট প্রিন্ট হবে
                printf("E++ Output -> %s\n", message);
            }
        } else {
            printf("E++ Error: Invalid print syntax. Use print(\"text\")\n");
        }
    } else {
        printf("E++ Error: Unknown command!\n");
    }
}

int main() {
    printf("=== E++ Language Engine Initialized ===\n");
    
    // আপনার E++ ভাষার টেস্ট কোড লাইন
    char sample_code[] = "print \"Hello E++ World!\"";
    printf("Executing: %s\n", sample_code);
    
    interpret_eplusplus(sample_code);
    
    return 0;
}
