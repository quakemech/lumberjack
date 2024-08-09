# Overview 

Lumberjack is a C logging library. This library allows code to log to syslog, 
stderr, a file, null, or a user provided function. 

# Dependencies 

This repo does not require any non-standard C libraries. 

# Compile 

To complile the library run: 

```bash
make
```

and to install to /usr/local/ type: 

```bash
make install
```

# Usage 

The repo includes a testbench.c file that can be used as an example. 

The library must first be instantiated using `log_init()`. The following exmple
creates a logging facility that will log to stderr with a priority of `LOG_ERR` 
that is defined in `syslog.h`.

```c
struct log_ctx = log_init("owner name", LDST_STDERR, LOG_ERR, 1, NULL, NULL);
```

The user can set the logging priority using: 

```c
log_set_priority(log, LOG_DEBUG);
```

Or the user can change the destination of the log 

```c
log_set_dst(log, LDST_FILE, /path/to/file, NULL);
```

To log directly to the library the user can issue `log_<>` commands such as:

```c
log_info(log, "Set value to %d", val);
```

The library logging macros can be called with the `log_ctx` pointer passed 
directly to the following log commands: 

- `log_dbg(log, ... )`
- `log_info(log, ... )`
- `log_notice(log, ... )`
- `log_warn(log, ... )`
- `log_err(log, ... )`

In many applications, the `struct log_ctx` pointer will be included as a field 
within another struct. In that circumstance, the user can use a difference set 
of macros to log commands. With these commands, the log statement macro assums 
the `struct log_ctx` pointer field is named `log` in the parent struct. The ctx
field in these is assumed to be the parent object pointer that holds the 
`struct log_ctx` pointer.

- `dbg(ctx, ... )`
- `info(ctx, ... )`
- `notice(ctx, ... )`
- `warn(ctx, ... )`
- `err(ctx, ... )`

To clean up the logging facility use `log_free()`

```c
log_free(log);
```

