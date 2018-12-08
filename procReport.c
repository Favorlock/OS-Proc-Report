#include<linux/init.h>
#include<linux/module.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include<linux/sched.h>
#include<linux/sched/signal.h>

#define procfs_name "proc_report"

static int proc_show(struct seq_file *m, void *v)
{
  struct task_struct *task;

  seq_printf(m, "PROCESS REPORT:\n");
  seq_printf(m, "proc_id,proc_name\n");

  for_each_process(task)
  {
    if (task->pid <= 650)
      continue;

    seq_printf(m, "%d,%s\n", task->pid, task->comm);
  }

  return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
  return single_open(file, proc_show, NULL);
}

static const struct file_operations proc_fops = {
  .owner = THIS_MODULE,
  .open = proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init proc_init (void)
{
  struct proc_dir_entry *proc_file_entry = proc_create(procfs_name, 0, NULL, &proc_fops);

  if (proc_file_entry == NULL)
    return -ENOMEM;

  printk(KERN_INFO "/proc/%s created\n", procfs_name);
  printk(KERN_INFO "procReport: kernel module initialized\n");
  return 0;
}

static void __exit proc_cleanup(void)
{
  remove_proc_entry(procfs_name, NULL);
  printk(KERN_INFO "/proc/%s removed\n", procfs_name);
  printk(KERN_INFO "procReport: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evan Lindsay");
MODULE_DESCRIPTION("TCSS 422 - Fall 2018 - Assignment 3");
MODULE_VERSION("0.01");
module_init(proc_init);
module_exit(proc_cleanup);
