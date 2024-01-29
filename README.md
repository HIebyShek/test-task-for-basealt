# Project Name

ttfb (Test Task for BaseALT)

## Description

This project consists of a shared library and a CLI utility that utilizes this library. The utility interacts with a public REST API to retrieve lists of binary packages from different branches of a database. It then performs comparisons between the package lists and generates a JSON output that displays the following information:

- Packages that exist in the first branch but not in the second branch.
- Packages that exist in the second branch but not in the first branch.
- Packages with a higher version-release in the first branch compared to the second branch.

The utility supports multiple branches and architectures, as specified in the API response.

## Dependencies

- json-c
- libcurl
- cmake 3.23 (build dependency)
- clang or gcc (C99 compiler, build dependency)
- g++ or clang++ (C++11 compiler, build dependency)

## Installation

### Arch Linux

To install the dependencies on Arch Linux, you can use the following command:

```shell
sudo pacman -S json-c libcurl cmake clang gcc
```

### ALT Linux

To install the dependencies on ALT Linux, you can use the package manager available at [https://packages.altlinux.org/ru/p10/](https://packages.altlinux.org/ru/p10/). Search for the respective packages and install them using the provided instructions.

## Building and Running the Utility

Follow the steps below to build and run the utility:

1. Clone the repository:

   ```shell
   git clone https://github.com/hiebyshek/test-task-for-basealt.git
   ```

2. Change into the project directory:

   ```shell
   cd test-task-for-basealt
   ```

3. Configure the project:

   For debug build:

   ```shell
   cmake --preset debug
   ```

   For release build:

   ```shell
   cmake --preset release
   ```

   This command configures the project based on the selected build type.

4. Build the project:

   For debug build:

   ```shell
   cmake --build --preset debug
   ```

   For release build:

   ```shell
   cmake --build --preset release
   ```

5. Run the utility:

   ```shell
   ./build/[debug/release]/ttfb/main [method] [branch1] [branch2]
   ```

   Replace `[method]` with the desired method (available methods: `-d`, `-dv`) and `[branch1]` and `[branch2]` with the desired branches (available branches: `sisyphus`, `p10`, `p9`).

   For example, to compare the packages using the difference method between the `sisyphus` and `p10` branches, you would run:

   ```shell
   ./build/[debug/release]/ttfb/main -d sisyphus p10
   ```

## Running Tests

To run the tests, you will need the C++11 compiler. You can use either g++ or clang++.

1. Run the tests:

   ```shell
   ./build/[debug/release]/libttfb/tests/ttfb_tests
   ```

## Repository History

The repository contains the complete history of all changes made during the development process, starting from the initial stage. The commit history has not been rewritten and is available for review.

Please note that the utility has been tested and verified on ALT Рабочая станция К 10.2. It is expected to work on other Linux distributions as well, provided that the dependencies are installed correctly.

## License

This project is licensed under the [MIT License](LICENSE).
