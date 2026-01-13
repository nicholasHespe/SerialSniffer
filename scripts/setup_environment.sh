#!/bin/bash
#
# SerialSniffer Complete Environment Setup
# Master installation script that sets up both firmware and analysis environments
#

set -e  # Exit on error

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

echo ""
echo "╔════════════════════════════════════════╗"
echo "║   SerialSniffer Environment Setup     ║"
echo "╚════════════════════════════════════════╝"
echo ""
echo "This script will set up your SerialSniffer development environment."
echo ""
echo "Components:"
echo "  1. Python Analysis Suite"
echo "  2. Teensy 4.1 Firmware Development"
echo ""
echo "Project Root: $PROJECT_ROOT"
echo ""

# Check system requirements
echo "Checking system requirements..."
echo ""

# Check for required commands
MISSING_DEPS=()

if ! command -v git &> /dev/null; then
    MISSING_DEPS+=("git")
fi

if ! command -v python3 &> /dev/null; then
    MISSING_DEPS+=("python3")
fi

if [ ${#MISSING_DEPS[@]} -ne 0 ]; then
    echo "ERROR: Missing required dependencies:"
    for dep in "${MISSING_DEPS[@]}"; do
        echo "  - $dep"
    done
    echo ""
    echo "Please install the missing dependencies and try again."
    exit 1
fi

echo "✓ Basic dependencies found"
echo ""

# Python Setup
echo "=========================================="
echo "Step 1: Python Analysis Suite Setup"
echo "=========================================="
echo ""

read -p "Install Python dependencies? [Y/n]: " install_python
install_python=${install_python:-Y}

if [[ $install_python =~ ^[Yy]$ ]]; then
    bash "$SCRIPT_DIR/install_python.sh"
else
    echo "Skipping Python setup"
fi

echo ""

# Teensy Setup
echo "=========================================="
echo "Step 2: Teensy 4.1 Development Setup"
echo "=========================================="
echo ""

read -p "Show Teensy setup instructions? [Y/n]: " show_teensy
show_teensy=${show_teensy:-Y}

if [[ $show_teensy =~ ^[Yy]$ ]]; then
    bash "$SCRIPT_DIR/install_teensy.sh"
else
    echo "Skipping Teensy setup instructions"
fi

echo ""

# Additional setup steps
echo "=========================================="
echo "Additional Setup Steps"
echo "=========================================="
echo ""

# Create example directories
echo "Creating project directories..."
mkdir -p "$PROJECT_ROOT/examples/captures"
mkdir -p "$PROJECT_ROOT/examples/analyses"
mkdir -p "$PROJECT_ROOT/tests/firmware_tests"
mkdir -p "$PROJECT_ROOT/tests/python_tests"
echo "✓ Directories created"
echo ""

# Git configuration
echo "Checking Git repository..."
if [ -d "$PROJECT_ROOT/.git" ]; then
    echo "✓ Git repository initialized"

    # Show current branch
    CURRENT_BRANCH=$(git -C "$PROJECT_ROOT" rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
    echo "  Current branch: $CURRENT_BRANCH"
else
    echo "! Git repository not initialized"
    read -p "Initialize Git repository? [y/N]: " init_git
    if [[ $init_git =~ ^[Yy]$ ]]; then
        cd "$PROJECT_ROOT"
        git init
        echo "✓ Git repository initialized"
    fi
fi
echo ""

# Summary
echo "╔════════════════════════════════════════╗"
echo "║        Setup Complete!                 ║"
echo "╚════════════════════════════════════════╝"
echo ""
echo "Next Steps:"
echo ""
echo "1. Firmware Development:"
echo "   - Open firmware/SerialSniffer/SerialSniffer.ino in Arduino IDE"
echo "   - Select Tools > Board > Teensy 4.1"
echo "   - Upload to your Teensy 4.1"
echo ""
echo "2. Python Analysis:"
echo "   - Activate virtual environment: source venv/bin/activate"
echo "   - Run analysis tool: serialsniffer --help"
echo "   - Analyze captures: serialsniffer analyze <file>"
echo ""
echo "3. Documentation:"
echo "   - README.md - Project overview"
echo "   - FUNCTIONAL_SPEC.md - Detailed specifications"
echo "   - docs/ - Additional documentation"
echo ""
echo "For more information, visit:"
echo "  https://github.com/nicholasHespe/SerialSniffer"
echo ""
