# Systems Programming Project: 'mysync'


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

4. **Explore Advanced Options**: 'mysync' offers a range of options to customize synchronization behavior. Use the `--help` option to view all available options and their descriptions.

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
