"""Launcher: pick a free port, start Waitress, open the browser."""

from __future__ import annotations

import socket
import sys
import threading
import time
import webbrowser

from waitress import serve

# Absolute imports (not relative): this module is the entry point both
# for `python -m pcl_designer` (where relative would work) AND for the
# PyInstaller-bundled binary (where it would not — PyInstaller runs the
# entry script as a standalone __main__ with no parent package).
from pcl_designer import __version__
from pcl_designer.app import app


def find_port(preferred: int = 8765) -> int:
    """Return `preferred` if free, otherwise let the OS pick a free port."""
    s = socket.socket()
    try:
        s.bind(("127.0.0.1", preferred))
        port = preferred
    except OSError:
        s.close()
        s = socket.socket()
        s.bind(("127.0.0.1", 0))
        port = s.getsockname()[1]
    finally:
        s.close()
    return port


def main() -> None:
    port = find_port()
    url = f"http://localhost:{port}"

    # Open browser after a short delay so Waitress is listening.
    def _open():
        time.sleep(0.8)
        try:
            webbrowser.open(url)
        except Exception:
            pass

    threading.Thread(target=_open, daemon=True).start()

    print("=" * 60)
    print(f"  PCL Designer v{__version__}")
    print(f"  Open in your browser: {url}")
    print(f"  Close this window (or Ctrl+C) to quit.")
    print("=" * 60)

    try:
        serve(app, host="127.0.0.1", port=port, threads=4)
    except KeyboardInterrupt:
        print("\nShutting down.")
        sys.exit(0)


if __name__ == "__main__":
    main()
