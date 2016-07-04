# liblog TODO list

###### Implementation

- [ ] Implement multithreading using message queue
- [ ] Use color and format manipulators from libut. (after they have been added)
- [ ] Use ut::string_view to avoid temporary std::strings when user supplies literal
- [x] Move the severity threshold from global to output based. This will make it possible to have log targets with different severity thresholds.
- [x] Maybe use ut::observer_ptr instead of raw pointers to make intent clear.
- [ ] Somehow allow both color and stream manipulators with the LOG_x() macros. (For example, ut::format should also work out of the box)
- [ ] Add file_target from ascii_ng to project.
- [ ] Allow custom tags that replace the severity string. (like "GLRenderer")
- [ ] Allow custom "tags" that get put somewhere in the log entry, for example to mark parts of large systems: "[sdl_render]    Fatal |". This should also allow custom colors.
