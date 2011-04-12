#!/usr/bin/python

class BitReader(object):
    def __init__(self, stream):
        self.stream = stream
        self.byte = 0
        self.nbits = 0

    def flush(self):
        self.nbits=0
        self.byte=0

    def sync(self, debug=False):
        self.nbits=0
        self.byte=0xff
        sank_bytes = 0
        while self.byte & ~0x98:
            self.byte = ord(self.stream.read(1))
            sank_bytes += 1
        if debug and sank_bytes > 1:
            print "resync: ate %d bytes" % (sank_bytes - 1)
        self.nbits = 8

    def get_bits(self, n=1):
        while n > self.nbits:
            x = self.stream.read(1)
            if x == "":
                print self.nbits
                print "%x" % self.byte
                raise EOFError

            x = ord(x)
            self.byte = (self.byte << 8) | x
            self.nbits += 8
        x = self.byte >> (self.nbits - n)
        self.byte -= x << (self.nbits - n)
        self.nbits -= n
        return x

    def decode(br, codetab, debug=False):
        stream = 0
        codeacc = 0
        have_bits = 0
    
        for code in codetab:
            get_bits = code[0] - have_bits
            codeacc <<= get_bits;
            codeacc += len(code[1])
        
            stream <<= get_bits;
            stream |= br.get_bits(get_bits);
    
            have_bits += get_bits;
    
            if stream < codeacc:
                return code[1][stream-codeacc]
        raise RuntimeError("unknown code encountered")


class STFParser(object):
    def __init__(self, stream):
        br = BitReader(stream)
        self.br = br
        magic = br.get_bits(16)
        if magic != 0x0100:
            raise RuntimeError("bad magic %x" % magic)

        version = stream.read(14)
        if version != "Anoto STF v1.0":
            raise RuntimeError("bad version %s" % version)

        speed = br.get_bits(16)
        self.speed = speed

    def get_force(self):
        tab_force = [
             [1, [0]]
            ,[6, [1,4,7,9,10,11,13,15,17,20,21,22,23,24,25,26,27,28,30]]
            ,[7, [2,3,5,6,8,12,14,16,18,19,29,31,32,33,34,35,36,49,52]]
            ,[8, [37,45,46,47,48,50,51,53,54]]
            ,[9, [38,39,40,41,44,55,56]]
            ,[10, [43,57,58]]
            ,[11, [42,59,60,61,62,63]]
            ]
        return self.br.decode(tab_force)
    
    def get_header2(self):
        tab_header2 = [
             [1, [0]]
            ,[2, [1,3]]
            ]
        return self.br.decode(tab_header2)
    
    def get_header(self):
        tab_header = [
             [1, [0]]
            ,[2, [1,2]]
            ]
        return self.br.decode(tab_header)
    
    def get_time(self):
        tab_time = [
             [1, [1]]
            ,[2, [2]]
            ,[4, [0,3,4]]
            ,[6, [5,6]]
            ,[7, [7,8]]
            ,[8, [9]]
            ,[9, [10,11]]
            ,[10, [12,13,14]]
            ,[11, [15]]
            ,[12, [16,17,18,19,21,22]]
            ,[13, [20,23,24,25,26,27]]
            ,[14, [28,29,30,31,32,35,36,37,40]]
            ,[15, [38,39,41,44,45,48,50,53,59,60,67,73,82,91,98,99,102]]
            ,[16, [33,34,42,43,46,47,49,51,52,54,55,56,57,58,61,62,63,64,65,66,68,69,70,71,72,74,75,76,77,78,79,80,81,83,84,85,86,87,88,89,90,92,93,94,95,96,97,100,101,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127]]
            ]
        return self.br.decode(tab_time)
    
    def get_deltax(self):
        tab_dx = [
             [4, [3,4,5]]
            ,[5, [0,1,2,6,7,8,9,-8,-7,-6,-5,-4,-3,-2,-1]]
            ,[6, [10,11,12,13,-13,-12,-11,-10,-9]]
            ,[7, [14,15,16,17,18,19,-18,-17,-16,-15,-14]]
            ,[8, [20,21,22,23,24,26,-25,-24,-23,-22,-21,-20,-19]]
            ,[9, [25,27,28,29,30,31,32,33,34,36,38,-36,-34,-33,-32,-31,-30,-29,-28,-27,-26]]
            ,[10, [35,37,39,40,41,42,43,44,45,46,47,49,55,-57,-50,-47,-46,-44,-43,-42,-41,-40,-39,-38,-37,-35]]
            ]
        return self.br.decode(tab_dx)
    
    def get_deltay(self):
        tab_dy = [
             [5, [0,1,2,3,4,5,6,7,8,-9,-8,-7,-6,-5,-4,-3,-2,-1]]
            ,[6, [9,10,11,12,13,-14,-13,-12,-11,-10]]
            ,[7, [14,15,16,17,18,19,20,-20,-19,-18,-17,-16,-15]]
            ,[8, [21,22,23,24,25,26,27,28,29,30,31,-27,-26,-25,-24,-23,-22,-21]]
            ,[9, [32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,49,-45,-44,-43,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28]]
            ,[10, [47,48,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,66,67,68,69,71,72,73,78,85,-75,-69,-64,-62,-61,-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-42,-41]]
            ]
        return self.br.decode(tab_dy)
    
    def get_header3(self):
        tab_header3 = [
             [1, [0,1]]
            ]
        return self.br.decode(tab_header3)
    
    def get_deltaforce(self):
        tab_dforce = [
             [1, [0]]
            ,[3, [1]]
            ,[4, [-1]]
            ,[5, [2,-2]]
            ,[6, [3,-3]]
            ,[7, [4,52,53,-4]]
            ,[8, [5,6,49,50,51,54,55,-17,-16,-15,-14,-13,-12,-7,-6,-5]]
            ,[9, [7,8,9,10,11,12,13,30,31,36,37,38,39,40,41,46,48,56,57,-56,-55,-54,-53,-52,-51,-50,-49,-40,-38,-21,-20,-19,-18,-11,-10,-9,-8]]
            ,[10, [14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35,42,43,44,45,47,58,-57,-48,-47,-46,-45,-44,-43,-42,-41,-39,-37,-36,-35,-34,-33,-32,-31,-29,-28,-27,-26,-25,-24,-23,-22]]
            ,[11, [-59,-58,-30]]
            ,[12, [59,-64]]
            ,[13, [60,61,62,63,-63,-62,-61,-60]]
            ]
        return self.br.decode(tab_dforce)

    def parse(self):
        start_time = 0
        br = self.br

        while True:
            br.sync()
            header = br.get_bits(8)

            if header == 0x18:
                time = br.get_bits(64)
            elif header == 0x10:
                time = br.get_bits(32)
            elif header == 0x08:
                time = br.get_bits(16)
            elif header == 0:
                time = br.get_bits(8)
            elif header == 0x80:
                break
            else:
                print "bad header %x" % header
                continue

            try:
                start_time += time
                x0 = br.get_bits(16)
                y0 = br.get_bits(16)
                f0 = self.get_force()

                self.handle_point(x0, y0, f0, start_time)

                xa=0
                ya=0
                while True:
                    header = self.get_header()
                    if header==0 or header==1:
                        time = self.get_time()
                    else:
                        header2 = self.get_header2(br)
                        if header2 == 0:
                            time = br.get_bits(8)
                        elif header2 == 1:
                            time = br.get_bits(16)
                        elif header2 == 2:
                            time = br.get_bits(32)
                        else:
                            print "bad stroke time header %d" % header2

                    if time==0:
                        self.handle_stroke_end(time)
                        break

                    do_delta = True
                    if header > 0:
                        len = br.get_bits()
                        if len:
                            do_delta = False
                            x1 = br.get_bits(16)
                            y1 = br.get_bits(16)

                            xa = x1 - x0
                            ya = y1 - y0
                        else:
                            deltax = br.get_bits(8)
                            deltay = br.get_bits(8)
                    else:
                        deltax = self.get_deltax()
                        deltay = self.get_deltay()

                    deltaf = self.get_deltaforce()

                    if do_delta:
                        xa = deltax + (xa * time) / 256
                        ya = deltay + (ya * time) / 256

                    x0 += xa
                    xa *= 256 / time

                    y0 += ya
                    ya *= 256 / time
                    f0 += deltaf 

                    self.handle_point(x0, y0, f0, start_time)
                    pass
            except Exception, e:
                print e
                continue
        pass

    def handle_stroke_end(self, time):
        pass

    def handle_point(self, x, y, force, time):
        print "Override STFParser and provide your own!"
        assert False

if __name__ == "__main__":
    import sys

    class TestParser(STFParser):
        def handle_point(self, x, y, f, time):
            print "%d, %d, %d, %d" % (x, y, f, time)

    f = file(sys.argv[1])
    tp = TestParser(f)
    print "Speed is %d" % tp.speed
    tp.parse()

