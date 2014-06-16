__author__ = 'adam'

import sys
import hashlib
import urllib2
import urllib
import bencode


def downloadTracker():
    if len(sys.argv) != 3:
        print("Improper usage!")
        exit(-1)
    src = sys.argv[1]
    f = open(src, "rb")
    tracker = f.read()
    f.close()
    decoded = bencode.bdecode(tracker)
    if not isinstance(decoded, bencode.DictType):
        print("Invalid bencode!")
        exit(-1)
    announce = decoded.get("announce")
    print(announce)
    decoded = decoded.get("info")
    name = decoded.get("name")
    print(name)
    pieceLength = decoded.get("piece length")
    print(pieceLength)
    length = decoded.get("length")
    print(length)
    pieces = decoded.get("pieces")
    getRequest=announce+"?"
    sha1 = hashlib.sha1()
    sha1.update(str(decoded))
    getRequest += "info_hash="+urllib.quote(sha1.digest())
    getRequest += "&peer_id=xxDOWNTORRDOWNTORRxx&port=6881&compact=0&uploaded=0&downloaded=0&left="+str(length)
    print(getRequest)
    response = urllib2.urlopen(getRequest)
    print(response.read())
    response.close()

def escape(hex):
    escaped = ""
    for i in range(0,(len(hex)/2)):
        escaped += '%' + str(hex[i*2]).upper() + str(hex[(i*2)+1]).upper()
    return escaped

def main():
    downloadTracker()

if __name__ == "__main__":
    main()
