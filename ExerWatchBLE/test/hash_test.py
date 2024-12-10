# Proper (fast) Python implementations of Dan Bernstein's DJB2 32-bit hashing function
#
# DJB2 has terrible avalanching performance, though.
# For example, it returns the same hash values for these strings: "xy", "yX", "z7".
# I recommend using Murmur3 hash. Or, at least, FNV-1a or SDBM hashes below.

import functools
def djb2(x): return functools.reduce(lambda x, c: 0xFFFFFFFF & (x*33 + c), x, 5381)

HASH_SIZE=751
hex(djb2(b'hello, world')) == '0xb0e4250d'

def hash_djb2(s):
    hash = 5381
    for x in s:
        hash = (((hash << 5) + hash) + ord(x)) & 0xFFFFFFFF
    return hash

hex(hash_djb2(u'hello world, 世界')) == '0xa6bd702f'

inpt = ""
while input != "q":
    inpt = input("Enter string to hash: ")
    h = hash_djb2(inpt)
    print(f"Hash: {h}")
    print(f"Hash modulo {HASH_SIZE}: {h % HASH_SIZE}")

