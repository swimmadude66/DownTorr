using MonoTorrent.BEncoding;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Downtorr
{
    class TorrentModel
    {
        String announce { get; set; }
        String name { get; set; }
        int peiceLength { get; set; }
        int length { get; set; }
        String urlList { get; set; }
        String infoHash { get; set; }

        public TorrentModel(BEncodedDictionary dict)
        {
            ICollection<BEncodedString> keys = dict.Keys;
            for (BEncodedValue item in dict){

            }
        }
    }
}
