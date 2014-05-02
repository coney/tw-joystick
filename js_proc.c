#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/export.h>

#include "js_log.h"
#include "js_dev.h"
#include "js_proc.h"

static char js_proc_buffer[JS_PROC_BUFSIZE];

static int js_proc_test_show(struct seq_file *m, void *v) {
    logdebug("proc read (/proc/%s) called\n", JS_PROC_TEST);
    seq_printf(m, "Hello Kitty!\n");
    return 0;
}

static int js_proc_test_open(struct inode *inode, struct file *file) {
    return single_open(file, js_proc_test_show, NULL);
}

static ssize_t js_proc_test_write(struct file *file, const char *buffer, size_t count, loff_t *offset) {
    int button, player;
    logdebug("write %zu bytes, offset %p\n", count, offset);
    if (count >= JS_PROC_BUFSIZE) {
        count = JS_PROC_BUFSIZE - 1;
    }

    if (copy_from_user(js_proc_buffer, buffer, count)) {
        return -EFAULT;
    }

    js_proc_buffer[count] = 0;

    logdebug("receive command %s\n", js_proc_buffer);
    if (sscanf(js_proc_buffer, "%d %d", &player, &button) == 2) {
        int ret = js_device_process(player, abs(button), !!(button > 0));
        logdebug("process player:%d button %d, result=%d\n", player, button, ret);
    }

    return count;
}

static const struct file_operations js_proc_config_fops = {
        .owner = THIS_MODULE,
        .open = js_proc_test_open,
        .read = seq_read,
        .write  = js_proc_test_write,
        .llseek = seq_lseek,
        .release = single_release,
};



static struct proc_dir_entry *js_proc_root = NULL;

int js_proc_init(void) {

    if ((js_proc_root = proc_mkdir_mode(JS_PROC_NAME, S_IRUGO | S_IWUGO, NULL)) == NULL
        || !proc_create(JS_PROC_TEST, S_IRUGO | S_IWUGO, js_proc_root, &js_proc_config_fops)) {
            js_proc_clear();
            return -ENOMEM;
    }

    return 0;
}

void js_proc_clear(void) {
    if (js_proc_root) {
        remove_proc_entry(JS_PROC_TEST, js_proc_root);
        remove_proc_entry(JS_PROC_NAME, NULL);
        js_proc_root = 0;
    }
}
