# Coding Rules

## 1. Language & Standard
- C99 (Zephyr default)
- No dynamic memory allocation (`malloc`/`free`) in application code
- Use Zephyr kernel APIs for all OS primitives

## 2. Naming Convention
- Functions: `module_name_action()` (e.g., `ble_service_init()`)
- Types: `snake_case_t` (e.g., `led_state_t`)
- Macros/Defines: `UPPER_SNAKE_CASE` (e.g., `TEMP_SCALE_FACTOR`)
- Static globals: `snake_case` with file scope only

## 3. File Structure
- Each module: `.c` + `.h` pair in `src/`
- Configuration defines: `include/app_config.h`
- Every `.h` has include guard: `#ifndef MODULE_H`

## 4. Error Handling
- All init functions return `int` (0 = success, negative errno = fail)
- Check return values, log errors with `LOG_ERR()`
- Never silently ignore errors

## 5. Thread Safety
- No shared mutable globals without protection
- Use `k_work` to defer ISR processing to thread context
- BLE notifications only from thread context (not ISR)

## 6. Logging
- Use Zephyr `LOG_MODULE_REGISTER()` per file
- Levels: ERR (errors), WRN (recoverable), INF (state changes), DBG (data values)

## 7. Comments
- Doxygen `@brief` for all public functions
- File header with `@file`, `@brief`, `@author`
- Inline comments for non-obvious logic only

## 8. Build
- Must compile with zero warnings (`-Wall -Werror`)
- Must pass `west build --pristine always`
