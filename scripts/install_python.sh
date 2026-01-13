#!/bin/bash
#
# SerialSniffer Python Analysis Suite Installation Script
# Installs Python dependencies and sets up the analysis environment
#

set -e  # Exit on error

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
PYTHON_DIR="$PROJECT_ROOT/python"

echo "=========================================="
echo "SerialSniffer Python Setup"
echo "=========================================="
echo ""

# Check Python version
echo "Checking Python installation..."
if ! command -v python3 &> /dev/null; then
    echo "ERROR: Python 3 is not installed."
    echo "Please install Python 3.8 or higher from https://www.python.org/"
    exit 1
fi

PYTHON_VERSION=$(python3 --version | cut -d' ' -f2 | cut -d'.' -f1,2)
echo "Found Python $PYTHON_VERSION"

# Check if Python version is >= 3.8
REQUIRED_VERSION="3.8"
if [ "$(printf '%s\n' "$REQUIRED_VERSION" "$PYTHON_VERSION" | sort -V | head -n1)" != "$REQUIRED_VERSION" ]; then
    echo "ERROR: Python $REQUIRED_VERSION or higher is required."
    echo "Current version: $PYTHON_VERSION"
    exit 1
fi

echo "Python version OK"
echo ""

# Check for pip
echo "Checking pip installation..."
if ! command -v pip3 &> /dev/null; then
    echo "ERROR: pip3 is not installed."
    echo "Please install pip for Python 3"
    exit 1
fi
echo "pip3 found"
echo ""

# Upgrade pip
echo "Upgrading pip..."
python3 -m pip install --upgrade pip
echo ""

# Create virtual environment (optional but recommended)
read -p "Create virtual environment? (recommended) [Y/n]: " create_venv
create_venv=${create_venv:-Y}

if [[ $create_venv =~ ^[Yy]$ ]]; then
    VENV_DIR="$PROJECT_ROOT/venv"

    if [ -d "$VENV_DIR" ]; then
        echo "Virtual environment already exists at $VENV_DIR"
    else
        echo "Creating virtual environment..."
        python3 -m venv "$VENV_DIR"
        echo "Virtual environment created at $VENV_DIR"
    fi

    echo ""
    echo "To activate the virtual environment, run:"
    echo "  source venv/bin/activate  (Linux/Mac)"
    echo "  venv\\Scripts\\activate     (Windows)"
    echo ""

    # Activate venv for this script
    source "$VENV_DIR/bin/activate" 2>/dev/null || . "$VENV_DIR/Scripts/activate" 2>/dev/null || true
fi

# Install requirements
echo "Installing Python dependencies..."
cd "$PYTHON_DIR"

if [ -f "requirements.txt" ]; then
    pip3 install -r requirements.txt
    echo "Dependencies installed successfully"
else
    echo "ERROR: requirements.txt not found in $PYTHON_DIR"
    exit 1
fi
echo ""

# Install package in development mode
echo "Installing SerialSniffer Analysis Suite..."
pip3 install -e .
echo ""

# Verify installation
echo "Verifying installation..."
if command -v serialsniffer &> /dev/null; then
    echo "SUCCESS: SerialSniffer command-line tool installed"
    serialsniffer --version
else
    echo "WARNING: serialsniffer command not found in PATH"
    echo "You may need to add the Python scripts directory to your PATH"
fi
echo ""

echo "=========================================="
echo "Python Setup Complete!"
echo "=========================================="
echo ""
echo "Quick Start:"
echo "  1. Activate virtual environment (if created):"
echo "     source venv/bin/activate"
echo ""
echo "  2. Run analysis tool:"
echo "     serialsniffer --help"
echo "     python3 python/SerialSnifferAnalysis.py --help"
echo ""
echo "  3. Analyze captured data:"
echo "     serialsniffer analyze <capture_file>"
echo ""
