// 我自己写的用于给瑞萨平台的BL2.BIN塞引导参数.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <bl2_path> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *bl2_path = argv[1];
    const char *output_file = argv[2];

    // 打开输入文件
    FILE *bl2_file = fopen(bl2_path, "rb");
    if (!bl2_file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    // 获取 bl2.bin 文件大小
    if (fseek(bl2_file, 0, SEEK_END) != 0) {
        perror("Failed to determine file size");
        fclose(bl2_file);
        return EXIT_FAILURE;
    }

    size_t bl2_size = ftell(bl2_file);
    if (bl2_size == (size_t)-1) {
        perror("Failed to read file size");
        fclose(bl2_file);
        return EXIT_FAILURE;
    }
    rewind(bl2_file); // 返回文件开头

    // 计算对齐后的大小
    size_t aligned_size = (bl2_size + 3) & ~3;

    // 准备对齐大小的小端格式
    uint8_t bl2_size_hex[512];
    memset(bl2_size_hex, 0xFF, sizeof(bl2_size_hex));
    bl2_size_hex[0] = aligned_size & 0xFF;
    bl2_size_hex[1] = (aligned_size >> 8) & 0xFF;
    bl2_size_hex[2] = (aligned_size >> 16) & 0xFF;
    bl2_size_hex[3] = (aligned_size >> 24) & 0xFF;

    // 填充最后两个字节
    bl2_size_hex[510] = 0x55;
    bl2_size_hex[511] = 0xAA;

    // 打开输出文件
    FILE *output = fopen(output_file, "wb");
    if (!output) {
        perror("Failed to open output file");
        fclose(bl2_file);
        return EXIT_FAILURE;
    }

    // 写入对齐大小段
    if (fwrite(bl2_size_hex, 1, 512, output) != 512) {
        perror("Failed to write header to output file");
        fclose(bl2_file);
        fclose(output);
        return EXIT_FAILURE;
    }

    // 缓冲区用于文件读取和写入
    uint8_t buffer[512];
    size_t bytes_read;

    // 将 bl2.bin 写入到输出文件
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), bl2_file)) > 0) {
        if (fwrite(buffer, 1, bytes_read, output) != bytes_read) {
            perror("Failed to write data to output file");
            fclose(bl2_file);
            fclose(output);
            return EXIT_FAILURE;
        }
    }

    // 检查是否读取时出错
    if (ferror(bl2_file)) {
        perror("Error reading input file");
        fclose(bl2_file);
        fclose(output);
        return EXIT_FAILURE;
    }

    // 关闭文件
    fclose(bl2_file);
    fclose(output);

    printf("Process completed successfully. Output: %s\n", output_file);
    return EXIT_SUCCESS;
}
