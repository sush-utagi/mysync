# Systems Programming Project: 'mysync'

## Overview
'mysync' is a lightweight file synchronization utility inspired by the 'rsync' tool. It provides a simple and reliable solution for synchronizing files and directories between two locations. 'mysync' is designed to be easy to use and efficient, making it ideal for basic file synchronization tasks.

## Features
- **Basic Synchronization**: 'mysync' allows you to synchronize files and directories between two locations using a simple command-line interface.
- **Efficient File Transfer**: Utilizing checksums and incremental file transfers, 'mysync' minimizes data transfer and ensures accurate synchronization.
- **Recursive Synchronization**: 'mysync' can be used recursively with any number of directories, allowing you to synchronize entire directory trees effortlessly.
- **Selective Synchronization**: 'mysync' supports include and exclude patterns, allowing you to selectively synchronize files and directories.
- **Backup and Restore**: Use 'mysync' to create backups of your files and easily restore them to a previous state.


## How to Run 'mysync'
1. **Clone the Repository**: Clone the 'mysync' repository to your local machine.
   ```bash
   git clone https://github.com/your-username/mysync.git
   ```
2. **Build 'mysync'**: Navigate to the 'mysync' directory and run the make command to build the executable.
   ```bash
   cd mysync
   make
   ```
3. **Run 'mysync'**: Use the following command to synchronize directories.
   ```bash
   ./mysync [options] directory1 directory2 [directory3 ...]
   ```
   - Replace `[options]` with any additional command-line options you want to use.
   - Replace `directory1`, `directory2`, etc., with the directories you want to synchronize.

4. **Explore Advanced Options**: 'mysync' offers a set ofoptions to customize synchronization behavior. Use the `--help` option to view all available options and their descriptions.

## Options

| Option          | Description                                      | Value            |
|-----------------|--------------------------------------------------|------------------|
| `-a`            | Synchronize all files, including hidden files    | 0/1              |
| `-i <pattern>`  | Specify pattern to ignore                        | `<pattern>`      |
| `-n`            | Disable synchronization (dry run)                | 0/1              |
| `-o <pattern>`  | Specify pattern to include only                  | `<pattern>`      |
| `-p`            | Preserve permissions, ownership, and timestamps  | 0/1              |
| `-r`            | Enable recursive synchronization                 | 0/1              |
| `-v`            | Enable verbose output                            | 0/1              |



## Example Usage
- Synchronize two directories:
  ```bash
  ./mysync dir1 dir2
  ```

- Synchronize directories with verbose output:
  ```bash
  ./mysync -v dir1 dir2
  ```

- Synchronize directories with include and exclude patterns:
  ```bash
  ./mysync --include='*.txt' --exclude='temp/' dir1 dir2
  ```

## Note
Please ensure that you have the necessary permissions to access and modify the specified directories. Use 'mysync' responsibly, and always double-check your synchronization commands to avoid accidental data loss.
