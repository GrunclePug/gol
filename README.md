# GoL Ncurses TUI (Cellular Automata)

A high-performance, self-managing **Terminal User Interface (TUI)** for running **C/A (Cellular Automata)** simulations, written in C and utilizing the ncurses library.

---

## Overview

This Cellular Automata Ncurses TUI has evolved beyond a simple Game of Life (GoL) implementation. The 0.2.0 release introduces a **dynamic, cyclic rule system** that allows the simulation to manage its own cell density.

The core concept is a continuous cycle between two configurable rule sets: **Growth** and **Decay**.

* It starts with a low-density, randomly scattered world.
* The **Growth** rule set is applied until the cell density reaches a configurable **Maximum Threshold**.
* It then automatically switches to the **Decay** rule set.
* The **Decay** rule set is applied until the cell density drops to a configurable **Minimum Threshold**.
* The cycle repeats, ensuring the world remains dynamic and prevents complete stagnation or complete overgrowth.

This leverages the **ncurses library** for a responsive grid and status bar, showcasing efficient memory management and classic C programming structure. The simulation features toroidal (wrapping) boundaries.

---

## Features

* **Terminal User Interface (TUI):** Runs entirely within the console using ncurses.
* **Toroidal Grid:** Cells wrap around the screen edges (top-to-bottom, left-to-right).
* **Efficient Update Logic:** Uses two grids (`current_grid` and `next_grid`) for rapid, safe generation updates.
* **Pattern Seeding:** Seeds the world with various stable and chaotic starting patterns (e.g., Glider, R-Pentomino).
* **Dynamic Resizing:** Supports terminal resizing on the fly (`KEY_RESIZE`).
* **Simple Controls:** Quit, Pause, and Step-by-Step generation advance.
* **Dynamic Rule Cycling:** Automatically switches between a **Growth** and **Decay** rule set based on current cell density.
  * **Default Growth Rule:** **B36/S236** (a high-density growth rule).
  * **Default Decay Rule:** **B3/S23** (the classic GoL rule set, which tends toward equilibrium/decay in this context).
* **Density Management:** Configurable **MIN\_THRESHOLD** and **MAX\_THRESHOLD** percentages dictate when the simulation cycles between rule sets.
* **Rule Customization:** Rule sets (Growth/Decay) are fully customizable via the `config.h` file (using the standard **B/S notation**).

---

## Requirements

* **C Compiler:** A standard C compiler (GCC or Clang).
* **GNU Make:** For building the project with the provided `Makefile`.
* **ncurses Library:** The development files for the ncurses library (e.g., `libncurses-dev` on Debian/Ubuntu, `ncurses-devel` on Fedora/CentOS, or installed via package manager on macOS/Arch Linux).

**Supported Systems:** Linux, macOS, and other Unix-like environments.

---

## Installation and Build

### Using Git (Recommended)

1.  Clone the repository:
  ```bash
  git clone [https://github.com/GrunclePug/gol.git](https://github.com/GrunclePug/gol.git)
  cd gol
  ```
2.  Compile the project using `make`:
  ```bash
  make
  ```
3.  Run the executable:
  ```bash
  ./gol
  # Optional: Run with a custom seeding density (0-1000, default is 35)
  # ./gol 10
  ```

### System-Wide Installation (make install)

To install the executable to a directory included in your system's PATH (defaulting to /usr/local/bin/ as defined in config.mk), use make install. This allows you to run the program simply by typing gol.
```bash
# You will likely need root privileges for this step
sudo make install
```

### Uninstallation

To remove the executable from your system:

```bash
# You will need root privileges for this step
sudo make uninstall
```

### From a Source Tarball

1.  Download the latest release tarball (`gol-X.X.tar.gz`).
2.  Extract the files:
  ```bash
  tar -xzf gol-X.X.tar.gz
  cd gol-X.X
  ```
3.  Compile and run using the steps above.

---

## Configuration and Custom Rules

All default settings, including the Growth and Decay rule sets and their cycle thresholds, are defined in config.h.

1.  Copy the default configuration (this is done by default on build if `config.h` doesn't exist):
  ```bash
  cp config.def.h config.h
  ```
2.  Edit `config.h` to customize the values. Key customizable variables include:

| Variable | Description | Default |
| :--- | :--- | :--- |
| `DEFAULT_DENSITY` | Initial cell density factor (1-1000) for random world seeding. | `2` |
| `DEFAULT_SPEED_MS` | Delay between generations in milliseconds. | `50` |
| `MIN_THRESHOLD` | Density (%) to trigger the switch to the **Growth** rule set. | `5` |
| `MAX_THRESHOLD` | Density (%) to trigger the switch to the **Decay** rule set. | `34` |
| `RULE_GROWTH_B/S` | Birth/Survival array for the Growth phase. | `B36/S236` |
| `RULE_DECAY_B/S` | Birth/Survival array for the Decay phase. | `B3/S23` |

  Note on Rules: The rule arrays use an index for the neighbor count (0-8) and a value of 1 for the rule to apply or 0 to ignore. For example, B36 is represented by {0, 0, 0, 1, 0, 0, 1, 0, 0}.
3.  Recompile the project:
  ```bash
  make clean
  make
  ```

---

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request.

---

## Author

Chad Humphries |
[Website](https://grunclepug.com/) |
[GitHub Profile](https://github.com/GrunclePug)

---

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
