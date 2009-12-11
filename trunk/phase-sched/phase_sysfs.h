/*
 * phase_sysfs.h
 *
 *  Created on: Dec 11, 2009
 *      Author: roseman
 */

#ifndef PHASE_SYSFS_H_
#define PHASE_SYSFS_H_

#include <linux/kobject.h>
#include <linux/sysfs.h>

struct phase_sysfs {
    struct subsystem subsystem;
    struct sysfs_ops phase_ops;
    struct attribute phase_attrs[2];
    struct kobj_type phase_ktype;
    struct phase_attr req;

};

struct phase_attr {
    struct attribute attr;
    ssize_t (*show)(void *obj, char *buffer);
    ssize_t (*store)(void *obj, const char *buffer, size_t size);
};

struct phase_kobj {
    struct kobject kobj;
    struct kobj_type ktype;
    struct sysfs_ops kops;
};

struct phase_kset {
    struct kset kset;
    struct kobj_type ktype;
    struct sysfs_ops kops;
};

#define DECL_PHASE_ATTR(name,mode,_show_func,_store_func) \
    static struct phase_attr name =  \
                            __ATTR(name,mode,_show_func,_store_func);

#define FUNC_SHOW_IATTR(_type,attr) \
    ssize_t _type##_show_##attr (void* obj, char *buf) \
    { \
        struct  _type *_obj = obj; \
        return sprintf(buf, "%d\n",_obj->attr); \
    }

#define FUNC_STORE_IATTR(_type,attr,min,max) \
    ssize_t _type##_store_##attr (void* obj, const char *buf, size_t size) \
    { \
        struct  _type *_obj = obj; \
        int val = skip_atoi(&buf); \
        if ((val < min) || (val > max)) \
            printk("Out of range [%d, %d] \n", min, max); \
        else \
            _obj->attr = val; \
        return size; \
    }

#define NAME_SHOW_IATTR(_type,attr) _type##_show_##attr
#define NAME_STORE_IATTR(_type,attr) _type##_store_##attr

#define DECL_PHASE_IATTR(objtype,attr,mode,min,max) \
            FUNC_SHOW_IATTR(objtype,attr) \
            FUNC_STORE_IATTR(objtype,attr,min,max) \
            DECL_PHASE_ATTR(attr,mode, \
                            NAME_SHOW_IATTR(objtype,attr), \
                            NAME_STORE_IATTR(objtype,attr))

#define FUNC_SHOW_SYSFS(_objtype) \
    static ssize_t _objtype##_show(struct kobject *kobj, \
                                struct attribute *attr, \
                                char *buffer) \
    { \
        struct phase_attr *_attr; \
        struct  _objtype *_obj; \
        struct phase_kset *mobj; \
        struct kset *kset; \
        if ((!kobj) || (!attr) || (!buffer)) \
            return 0; \
        kset = container_of(kobj, struct kset, kobj); \
        mobj = container_of(kset, struct phase_kset, kset); \
        _obj = container_of(mobj, struct _objtype, mobj); \
        _attr = container_of(attr, struct phase_attr, attr); \
        return _attr->show((void*)_obj, buffer); \
    }

#define FUNC_STORE_SYSFS(_objtype) \
    static ssize_t _objtype##_store(struct kobject *kobj, \
                                    struct attribute *attr, \
                                    const char *buffer, \
                                    size_t size) \
    { \
        struct phase_attr *_attr; \
        struct  _objtype *_obj; \
        struct phase_kset *mobj; \
        struct kset *kset; \
        if ((!kobj) || (!attr) || (!buffer)) \
            return 0; \
        kset = container_of(kobj, struct kset, kobj); \
        mobj = container_of(kset, struct phase_kset, kset); \
        _obj = container_of(mobj, struct _objtype, mobj); \
        _attr = container_of(attr, struct phase_attr, attr); \
        return _attr->store((void*)_obj, buffer, size); \
    }

#define NAME_SHOW_SYSFS(_type) _type##_show
#define NAME_STORE_SYSFS(_type) _type##_store

#endif /* PHASE_SYSFS_H_ */
