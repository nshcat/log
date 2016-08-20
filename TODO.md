# liblog TODO list

###### Implementation

- [ ] Maybe allow options/settings. Like automated blank line after LOCK/UNLOCK block. The tag-based system from libcl could be used for that. (e.g. `lg::logger::init( lg::target(...) )`)
- [ ] Use ut::string_view to avoid temporary std::strings when user supplies literal
- [ ] Add file_target from ascii_ng to project.
- [ ] Allow custom "tags" that get put somewhere in the log entry, for example to mark parts of large systems: "[sdl_render]    Fatal |". This should also allow custom colors. They could replace timestamp.
