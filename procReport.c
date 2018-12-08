#include<linux/init.h>
#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include<linux/sched.h>
#include<linux/sched/signal.h>
#include<linux/mm.h>

#define procfs_name "proc_report"

static unsigned long virt2phys(struct mm_struct *mm, unsigned long vpage)
{
  pgd_t *pgd;
  p4d_t *p4d;
  pud_t *pud;
  pmd_t *pmd;
  pte_t *pte;
  struct page *page;
  unsigned long physical_page_addr;

  pgd = pgd_offset(mm, vpage);
  if (pgd_none(*pgd) || pgd_bad(*pgd)) {
    return 0;
  }

  p4d = p4d_offset(pgd, vpage);
  if (p4d_none(*p4d) || p4d_bad(*p4d)) {
    return 0;
  }

  pud = pud_offset(p4d, vpage);
  if (pud_none(*pud) || pud_bad(*pud)) {
    return 0;
  }

  pmd = pmd_offset(pud, vpage);
  if (pmd_none(*pmd) || pmd_bad(*pmd)) {
    return 0;
  }

  if (!(pte = pte_offset_map(pmd, vpage))) {
    return 0;
  }

  if (!(page = pte_page(*pte))) {
    return 0;
  }

  physical_page_addr = page_to_phys(page);
  pte_unmap(pte);

  return physical_page_addr;
}

static void process_task(struct seq_file *m, struct task_struct *task)
{
  struct vm_area_struct *vma = 0;
  unsigned long vpage;
  unsigned long contigPages = 0;
  unsigned long nonContigPages = 0;
  unsigned long totalPages = 0;

  if (task->mm && task->mm->mmap) {
    for (vma = task->mm->mmap; vma; vma = vma->vm_next) {
      for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
        unsigned long physical_page_addr = virt2phys(task->mm, vpage);
        if (physical_page_addr == 0) {
          continue;
        }

        ++totalPages;
      }
    }
  }

  seq_printf(m, "%d,%s,%lu,%lu,%lu\n", task->pid, task->comm, contigPages, nonContigPages, totalPages);
}

static int proc_report(struct seq_file *m) {
  struct task_struct *task;

  seq_printf(m, "PROCESS REPORT:\n");
  seq_printf(m, "proc_id,proc_name,contig_pages,noncontig_pages,total_pages\n");

  for_each_process(task)
  {
    if (task->pid <= 650) {
      continue;
    }

    process_task(m, task);
  }

  return 0;
}

static int proc_show(struct seq_file *m, void *v)
{
  return proc_report(m);
}

static int proc_open(struct inode *inode, struct file *file)
{
  return single_open(file, proc_show, NULL);
}

static const struct file_operations proc_file_fops = {
  .owner = THIS_MODULE,
  .open = proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init init_callback (void)
{
  struct proc_dir_entry *proc_file_entry = proc_create(procfs_name, 0, NULL, &proc_file_fops);

  if (proc_file_entry == NULL) {
    return -ENOMEM;
  }

  printk(KERN_INFO "/proc/%s created\n", procfs_name);
  printk(KERN_INFO "procReport: kernel module initialized\n");
  return 0;
}

static void __exit cleanup_callback(void)
{
  remove_proc_entry(procfs_name, NULL);
  printk(KERN_INFO "/proc/%s removed\n", procfs_name);
  printk(KERN_INFO "procReport: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evan Lindsay");
MODULE_DESCRIPTION("TCSS 422 - Fall 2018 - Assignment 3");
MODULE_VERSION("0.01");
module_init(init_callback);
module_exit(cleanup_callback);
