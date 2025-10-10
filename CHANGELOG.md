# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0] - 2025-10-10

### Added

- Window state persistence with `WindowState` struct (position, size, maximized status)
- `nlohmann-json` dependency for serialization
- Pre-commit hooks configuration with code formatting, linting, and security checks
- GitHub Actions workflow for automated pre-commit checks

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

[0.3.0]: https://github.com/Konstantysz/kappa-core/compare/v0.2.2...v0.3.0
[0.2.2]: https://github.com/Konstantysz/kappa-core/compare/v0.2.1...v0.2.2
[0.2.1]: https://github.com/Konstantysz/kappa-core/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/Konstantysz/kappa-core/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/Konstantysz/kappa-core/releases/tag/v0.1.0
