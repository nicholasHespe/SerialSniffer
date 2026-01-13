"""
SerialSniffer Analysis Suite Setup
"""

from setuptools import setup, find_packages
from pathlib import Path

# Read the README file
readme_file = Path(__file__).parent.parent / "README.md"
long_description = readme_file.read_text() if readme_file.exists() else ""

setup(
    name="serialsniffer-analysis",
    version="0.1.0",
    author="SerialSniffer Team",
    description="Analysis suite for SerialSniffer captured serial data",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/nicholasHespe/SerialSniffer",
    packages=find_packages(),
    py_modules=["SerialSnifferAnalysis"],
    install_requires=[
        "pandas>=2.0.0",
        "numpy>=1.24.0",
        "matplotlib>=3.7.0",
        "seaborn>=0.12.0",
        "pyserial>=3.5",
        "openpyxl>=3.1.0",
        "crcmod>=1.7",
        "click>=8.1.0",
        "rich>=13.0.0",
    ],
    extras_require={
        "dev": [
            "pytest>=7.4.0",
            "pytest-cov>=4.1.0",
            "black>=23.0.0",
            "flake8>=6.0.0",
            "mypy>=1.0.0",
        ]
    },
    entry_points={
        "console_scripts": [
            "serialsniffer=SerialSnifferAnalysis:cli",
        ],
    },
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Embedded Systems",
        "Topic :: System :: Hardware",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
    ],
    python_requires=">=3.8",
)
