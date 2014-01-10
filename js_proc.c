#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include "js_log.h"

#include "js_proc.h"

static struct proc_dir_entry *s_proc_entry = NULL;
static char *s_proc_buffer = NULL;

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
    if (copy_from_user(s_proc_buffer, buffer, count)) {
        return -EFAULT;
    }

    s_proc_buffer[JS_PROC_BUFSIZE - 1] = 0;
    logdebug("%s\n", s_proc_buffer);
    return count;
}

int js_init_proc_entry(void)
{
    s_proc_buffer = kmalloc(JS_PROC_BUFSIZE, GFP_KERNEL);
    if (!s_proc_buffer) {
        goto error;
    }

    s_proc_entry = create_proc_entry(JS_PROC_NAME, 0644, NULL);
    if (!s_proc_entry) {
        goto error;
    }

    s_proc_entry->read_proc = js_proc_read;
    s_proc_entry->write_proc = js_proc_write;
    s_proc_entry->mode = S_IFREG | S_IRUGO;
    s_proc_entry->uid = s_proc_entry->gid = 0;

    return 0;

error:
    js_clear_proc_entry();
    return -ENOMEM;
}

void js_clear_proc_entry(void)
{
    if (s_proc_entry)
    {
        remove_proc_entry(JS_PROC_NAME, NULL);
        s_proc_entry = NULL;
    }
    if (s_proc_buffer)
    {
        kfree(s_proc_buffer);
        s_proc_buffer = NULL;
    }
}