import pytest
import os
import re


def load_result():
    """Load result.txt and return its content."""
    assert os.path.exists('result.txt'), \
        "result.txt not found — run: ./a.out > result.txt"
    with open('result.txt', 'r') as f:
        return f.read()


def extract_addresses(content, label):
    """Find all hex addresses in a section matching the label.
    A section starts at a line containing the label and continues
    until a blank line or next section header (--- or ===)."""
    addrs = []
    lines = content.split('\n')
    in_section = False
    for line in lines:
        if label.lower() in line.lower():
            in_section = True
            # also grab any address on the header line itself
            found = re.findall(r'0x[0-9a-fA-F]+', line)
            addrs.extend(found)
            continue
        if in_section:
            if line.strip() == '' or line.strip().startswith('---') or line.strip().startswith('==='):
                in_section = False
                continue
            found = re.findall(r'0x[0-9a-fA-F]+', line)
            addrs.extend(found)
    return [int(a, 16) for a in addrs]


@pytest.mark.T1
def test_segment_labels():
    """T1: result.txt contains all 5 segment labels"""
    content = load_result()
    print(f"Output length: {len(content)} chars")

    for label in ['TEXT', 'DATA', 'BSS', 'STACK', 'HEAP']:
        found = label.lower() in content.lower()
        assert found, f"Missing segment label: {label}"
        print(f"PASS: found '{label}' in output")

    print("PASS: all 5 segment labels present")


@pytest.mark.T2
def test_hex_addresses():
    """T2: result.txt contains at least 2 hex addresses per segment"""
    content = load_result()

    for label in ['TEXT', 'DATA', 'BSS', 'STACK', 'HEAP']:
        addrs = extract_addresses(content, label)
        assert len(addrs) >= 2, \
            f"{label}: expected at least 2 hex addresses, found {len(addrs)}"
        print(f"PASS: {label} has {len(addrs)} addresses: {[hex(a) for a in addrs]}")

    print("PASS: all segments have at least 2 hex addresses")


@pytest.mark.T3
def test_stack_grows_down():
    """T3: output shows stack grows down (requires cross-function comparison)"""
    content = load_result()
    addrs = extract_addresses(content, 'STACK')
    assert len(addrs) >= 3, \
        f"STACK: need at least 3 addresses (main local, param, function local), found {len(addrs)}"
    print(f"STACK addresses: {[hex(a) for a in addrs]}")

    # Check program's own comparison output
    lower = content.lower()
    has_down = 'stack grows' in lower and 'down' in lower
    assert has_down, \
        "Output must show 'Stack grows: DOWN' — compare address from main() vs address in child function"
    print("PASS: output confirms stack grows down")


@pytest.mark.T4
def test_heap_grows_up():
    """T4: output shows heap grows up"""
    content = load_result()
    addrs = extract_addresses(content, 'HEAP')
    assert len(addrs) >= 2, \
        f"HEAP: need at least 2 addresses, found {len(addrs)}"
    print(f"HEAP addresses: {[hex(a) for a in addrs]}")

    # Check program's own comparison output
    lower = content.lower()
    has_up = 'heap grows' in lower and 'up' in lower
    assert has_up, \
        "Output must show 'Heap grows: UP' — compare addresses of two heap allocations"
    print("PASS: output confirms heap grows up")
