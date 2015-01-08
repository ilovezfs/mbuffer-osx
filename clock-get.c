
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <sys/time.h>

#include <semaphore.h>


struct timespec ts;

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time


int mac_sem_init(sem_t *sem, int pshared, int value)
{
    char *fname = strdup("/tmp/.mbuffer.XXXXXX");
    mktemp(fname);
    *sem = sem_open(fname, O_CREAT, 0644, value);
    unlink(fname);
    free(fname);
    return 0;
}

int mac_sem_destroy(sem_t *sem)
{
    sem_close(sem);
    return 0;
}

int mac_sem_wait(sem_t *sem)
{
    return sem_wait(*sem);
}

int mac_sem_post(sem_t *sem)
{
    return sem_post(*sem);
}

int mac_sem_getvalue(sem_t *sem, int *value)
{
    *value = 0;
    return 0;
}


int clock_gettime(unsigned long int id, struct timespec *ts)
{
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
    return 0;
}


#endif
