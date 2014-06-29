using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Downtorr
{
    class Program
    {
        static void Main(string[] args)
        {
            String contents = null;
            try
            {
                using (StreamReader reader = new StreamReader(args[1]))
                {
                    contents = reader.ReadToEnd();
                }
            }
            catch (Exception e)
            {
                throw (e);
            }

            //TorrentModel model = new TorrentModel(DecodeTorrent(contents));
            //Announce ann = parseAnnounce(model);
            //PeerResponse r = get(ann);
            //List<Peer> peers = Decode(r);
            //for (Peer p : peers){
                //Response r = handshake(peer);
                //download(r);
            //}
        }
    }
}
