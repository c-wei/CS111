#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 remaining_time;
  u32 waiting_time;
  u32 response_time;
  bool started;
  u32 quantum_used;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
  for(u32 i = 0; i < size; i++){
    data[i].remaining_time = data[i].burst_time;
    data[i].waiting_time = 0;
    data[i].response_time = 0;
    data[i].started = false;
    data[i].quantum_used = 0;
  }

  u32 current_time = 0;
  u32 finished = 0;
  u32 next_process = 0;
  struct process *current_proc = NULL;

  while(finished<size){
    while (next_process < size && data[next_process].arrival_time <=current_time){
      TAILQ_INSERT_TAIL(&list, &data[next_process], pointers);
      next_process++;
    }

    if(!current_proc && !TAILQ_EMPTY(&list)){
      current_proc = TAILQ_FIRST(&list);
      TAILQ_REMOVE(&list, current_proc, pointers);

      if(!current_proc -> started){
        current_proc->started = true;
        current_proc->response_time = current_time - current_proc->arrival_time;
        total_response_time += current_proc->response_time;
      }
    }

      if(current_proc){
        current_proc->remaining_time--;
        current_proc->quantum_used++;
        struct process *p;
        TAILQ_FOREACH(p, &list, pointers){
          p->waiting_time++;
        }

        if(current_proc->remaining_time == 0){
          total_waiting_time += current_proc->waiting_time;
          finished++;
          current_proc=NULL;
        } else if(current_proc->quantum_used >= quantum_length){
          TAILQ_INSERT_TAIL(&list, current_proc, pointers);
          current_proc -> quantum_used = 0;
          current_proc = NULL;
        }
      }
      current_time++;
      if(!current_proc && TAILQ_EMPTY(&list) && next_process < size){
        current_time = data[next_process].arrival_time;
      }
  }

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
