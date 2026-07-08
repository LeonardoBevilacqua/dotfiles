#include <inttypes.h>
#include <mach/mach.h>
#include <stdio.h>

int get_cpu_ticks(uint64_t ticks[4]) {
    host_cpu_load_info_data_t cpuinfo;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;

    kern_return_t kr = host_statistics(
        mach_host_self(),
        HOST_CPU_LOAD_INFO,
        (host_info_t)&cpuinfo,
        &count
    );

    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "host_statistics failed: %d\n", kr);
        return -1;
    }

    ticks[0] = cpuinfo.cpu_ticks[CPU_STATE_USER];
    ticks[1] = cpuinfo.cpu_ticks[CPU_STATE_SYSTEM];
    ticks[2] = cpuinfo.cpu_ticks[CPU_STATE_IDLE];
    ticks[3] = cpuinfo.cpu_ticks[CPU_STATE_NICE];

    return 0;
}

int main(void) {
    uint64_t t1[4];

    if (get_cpu_ticks(t1) != 0)
        return 1;

    uint64_t user   = t1[0];
    uint64_t system = t1[1];
    uint64_t idle   = t1[2];
    uint64_t nice   = t1[3];

    printf("cpu ");
    printf("%"PRIu64" ", user);
    printf("%"PRIu64" ", nice);
    printf("%"PRIu64" ", system);
    printf("%"PRIu64" ", idle);
    printf("0 0 0 0 0 0\n");

    return 0;
};
