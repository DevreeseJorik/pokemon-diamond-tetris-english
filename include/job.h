#ifndef _JOB_H
#define _JOB_H

#include "common.h"

#define pp_jobsys (0x021c6118 + 0x18)
#define fp_addJob 0x0201b621
#define fp_deleteJob 0x0201b6a1

typedef struct _Job
{
    u8 data[0x1c];
} Job;

/**
 * @brief Type of function registered as a job.
 * Receives a pointer to the registered job and a pointer to the working area used.
 */
typedef void (*jobFunc)(Job *, void *);

/**
 * Add a job
 * @param func Function to be executed
 * @param work Pointer to the working area used by func
 * @param priority Execution priority, with smaller values executed first
 * @return Pointer to the created Job structure
 * @details When a function is registered as a job, it will be called once per frame.
 */
static inline Job *addJob(jobFunc func, void *work, u32 priority)
{
    return ((Job * (*)(void *, jobFunc, void *, u32)) fp_addJob)(*((void **)pp_jobsys), func, work, priority);
}

/**
 * Delete a job
 * @param job Pointer to the job to be deleted
 */
static inline void deleteJob(Job *job)
{
    ((void (*)(Job *))fp_deleteJob)(job);
}

#endif // _JOB_H
