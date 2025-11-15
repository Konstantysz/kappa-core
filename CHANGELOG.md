# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.5.0] - 2025-11-15

### Added

- CI/CD vcpkg binary caching with `lukka/run-vcpkg@v11` for 5-10x faster builds
- Automated coverage workflow with HTML reports and PR comments
- Coverage statistics extraction and display in pull requests
- `Application::GetLayers()` returning `std::span<const std::unique_ptr<Layer>>` for read-only layer access

### Changed

- `BUILD_EXAMPLES` and `BUILD_TESTS` now default to OFF when used as submodule (standard CMake pattern)
- `Window::GetPosition()` now returns `Position{x, y}` instead of using out-parameters
- `Window::GetSize()` now returns `Size{width, height}` instead of using out-parameters
- `Window::GetState()` now uses C++20 designated initializers for clarity
- OpenGL version constants and timestep limits are now `constexpr`
- Magic numbers replaced with named `constexpr` constants with explanatory comments

### Fixed

- Python Unicode encoding in coverage script (Windows CI compatibility)
- Coverage workflow build directory paths
- Test executable name in coverage extraction

## [0.4.0] - 2025-10-18

### Added

- 81 unit tests with Google Test (73 passing)
- LLVM coverage support (~39% coverage)
- Variadic `PushLayer<T>(Args&&... args)` for dependency injection
- Coverage automation with `scripts/run-coverage.py`

## [0.3.1] - 2025-10-11

### Added

- `GetWindow()` methods to Application class

## [0.3.0] - 2025-10-10

### Added

- Window state persistence with `WindowState` struct (position, size, maximized status)
- Window control methods: `GetState()`, `SetState()`, `GetPosition()`, `SetPosition()`, `GetSize()`, `SetSize()`
- Window maximization methods: `IsMaximized()`, `Maximize()`, `Restore()`, `Center()`
- `WindowStatePersistence` utility class for JSON-based window state persistence with `Load()`, `Save()`, `LoadAndApply()`, and `CaptureAndSave()` methods
- `nlohmann-json` dependency for JSON serialization
- Pre-commit hooks configuration with:
  - General code quality checks (trailing whitespace, EOF, YAML/JSON validation, large files, case conflicts, merge conflicts)
  - C++ code formatting via clang-format
  - CMake formatting and linting via cmake-format
- GitHub Actions workflow for automated pre-commit checks on CI
- GitHub workflow status badges in README (build, tests, format, pre-commit, static analysis, documentation)

## [0.2.2] - 2025-10-07

### Fixed

- Logger now correctly reports source location of actual log macro call site instead of Logger.h

## [0.2.1] - 2025-10-04

### Fixed

- OpenGL context creation now falls back from 4.5 to 4.2 for WSL/WSLg compatibility
- Added runtime detection and graceful fallback for different OpenGL version support
- Improved logging to show actual OpenGL version obtained

## [0.2.0] - 2025-10-03

### Added

- Version management system with Version.h header and CMake project versioning
- vcpkg.json for package manager dependency management
- GitHub Actions CI/CD workflows:
  - Multi-platform builds (Windows, Ubuntu, macOS)
  - Automated testing
  - clang-format code style checks
  - Static analysis with clang-tidy and cppcheck
  - Doxygen API documentation generation with GitHub Pages deployment
- Comprehensive documentation:
  - ARCHITECTURE.md with design patterns, diagrams, and data flow explanations
  - EXAMPLES.md with detailed usage guides and tutorials
- Additional example applications:
  - Layers example demonstrating multi-layer architecture
  - Events example showcasing EventBus with custom events
- Code quality configuration files:
  - .clang-format for consistent code formatting
  - .clang-tidy for static analysis rules
- Organized project structure with src/ and include/ directories

### Changed

- **BREAKING**: Renamed namespace from `Core` to `Kappa` throughout the codebase
- **BREAKING**: Renamed CMake library target from `Core` to `Kappa`
- Reorganized source code into standard src/ and include/ directory structure
- Updated all examples to use new namespace and library name

### Fixed

- Added virtual destructor to `Layer` class to prevent memory leaks in derived classes
- Initialized `Window::handle` member to nullptr to prevent undefined behavior
- Configured clang-tidy as non-blocking due to GCC 14 standard library incompatibility with clang-tidy-14

## [0.1.0] - 2025-10-03

### Added

- Initial project structure
- Core framework components
- CMake build system
- Example application demonstrating framework usage

[0.4.0]: https://github.com/Konstantysz/kappa-core/compare/v0.3.1...v0.4.0
[0.3.1]: https://github.com/Konstantysz/kappa-core/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/Konstantysz/kappa-core/compare/v0.2.2...v0.3.0
[0.2.2]: https://github.com/Konstantysz/kappa-core/compare/v0.2.1...v0.2.2
[0.2.1]: https://github.com/Konstantysz/kappa-core/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/Konstantysz/kappa-core/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/Konstantysz/kappa-core/releases/tag/v0.1.0
