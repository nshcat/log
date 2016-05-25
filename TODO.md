# liblog TODO list

###### Implementation

- [ ] Use color and format manipulators from libut. (after they have been added)
- [ ] Use ut::string_view to avoid temporary std::strings when user supplies literal
- [x] Move the severity threshold from global to output based. This will make it possible to have log targets with different severity thresholds.
- [ ] Maybe use ut::observer_ptr instead of raw pointers to make intent clear.
- [ ] Somehow allow both color and stream manipulators with the LOG_x() macros. (For example, ut::format should also work out of the box)
- [ ] Allow custom "tags" that get put somewhere in the log entry, for example to mark parts of large systems: "[sdl_render]    Fatal |". This should also allow custom colors.