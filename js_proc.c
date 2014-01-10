#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include "js_log.h"
#include "js_proc.h"

static struct proc_dir_entry *proc_entry = NULL;
static char *proc_buffer = NULL;

int js_proc_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;

    printk("procfile_read (/proc/%s) called\n", JS_PROC_NAME);

    if (offset > 0) {
        ret = 0;
    }
    else {
        const char * hello = "Hello Kitty\n";
        unsigned int len = strlen(hello);
        memcpy(buffer, hello, len);
        ret = len;
    }

    return ret;
}

int js_proc_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
    if (count > JS_PROC_BUFSIZE)
    {
        count = JS_PROC_BUFSIZE;
    }

    /* write data to the buffer */
    if (copy_from_user(proc_buffer, buffer, count)) {
        return -EFAULT;
    }

    proc_buffer[JS_PROC_BUFSIZE - 1] = 0;
    logdebug("%s\n", proc_buffer);
    return count;
}

int js_proc_init(void)
{
    proc_buffer = kmalloc(JS_PROC_BUFSIZE, GFP_KERNEL);
    if (!proc_buffer) {
        goto error;
    }

    proc_entry = create_proc_entry(JS_PROC_NAME, 0644, NULL);
    if (!proc_entry) {
        goto error;
    }

    proc_entry->read_proc = js_proc_read;
    proc_entry->write_proc = js_proc_write;
    proc_entry->mode = S_IFREG | S_IRUGO;
    proc_entry->uid = proc_entry->gid = 0;

    return 0;

error:
    js_proc_clear();
    return -ENOMEM;
}

void js_proc_clear(void)
{
    if (proc_entry)
    {
        remove_proc_entry(JS_PROC_NAME, NULL);
        proc_entry = NULL;
    }
    if (proc_buffer)
    {
        kfree(proc_buffer);
        proc_buffer = NULL;
    }
}