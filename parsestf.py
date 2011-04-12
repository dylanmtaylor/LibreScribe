#!/usr/bin/python

class BitReader(object):
    def __init__(self, stream):
        self.stream = stream
        self.byte_array = self.stream.read()
        self.index = 0
        self.byte = 0
        self.nbits = 0

    def flush(self):
        self.nbits=0
        self.byte=0

    def get_bits(self, n=1):
        while n > self.nbits:
            if self.index >= len(self.byte_array):
                print self.nbits
                print "%x" % self.byte
                raise EOFError

            x = self.byte_array[self.index]
            self.index += 1
            x = ord(x)
            self.byte = (self.byte << 8) | x
            self.nbits += 8
        x = self.byte >> (self.nbits - n)
        self.byte -= x << (self.nbits - n)
        self.nbits -= n
        return x

def decode(codebook, br, debug=False):
    def max(x, xs):
        if len(xs) == 0:
            return x
        y = max(xs[0], xs[1:])
        if x > y:
            return x
        else:
            return y
    keys = codebook.keys()
    keys = map(len, keys)
    maxlen = max(keys[0], keys[1:])

    code = ""
    while code not in codebook:
	if len(code) > maxlen:
	    raise RuntimeError("code too long")
        bit = br.get_bits()
        if bit:
            code += "1"
        else:
            code += "0"
        if debug:
            print code
    return codebook[code]

def get_force(br):
    codebook = {
            '0': 0,
            '110001': 28,
            '110010': 30,
    }
    f = decode(codebook, br)
    return f*2

def get_time(br):
    codebook = {
            '0': 1,
	    '10': 2,
	    '1101': 3,
	    '1110': 4,
	    '111101': 6,
	    '1111100': 7,
	    '1111101': 8,
            '1100': 0,
	    '111111010': 10,
	    '111111011': 11,
	    '1111111000': 12,
	    '11111110110': 15,
    }
    return decode(codebook, br)

def get_deltax(br):
    codebook = {
            '00110': 0,
            '00111': 1,
            '10100': -1,
            '01000': 2,
            '10011': -2,
            '0000' : 3,
            '10010': -3,
            '0001' : 4,
            '10001': -4,
            '0010' : 5,
            '10000': -5,
            '01001': 6,
            '01111': -6,
            '01010': 7,
            '01110': -7,
            '01011': 8,
            '01101': -8,
            '01100': 9,
            '110010': -9,
            '101010': 10,
            '110001': -10,
            '101011': 11,
            '110000': -11,
            '101100': 12,
            '101111': -12,
            '101101': 13,
            '101110': -13,
	    '1100110': 14,
	    '1110000': -14,
            '1100111': 15,
            '1101111': -15,
            '1101000': 16,
            '1101110': -16,
            '1101001': 17,
            '1101101': -17,
            '1101010': 18,
            '1101100': -18,
            '1101011': 19,
            '11101110': -19,
	    '11100010': 20,
	    '11101101': -20,
	    '11101100': -21,
	    '11100100': 22,
	    '11101011': -22,
	    '11101010': -23,
	    '11100110': 24,
            '11100011': 25,
            '11101000': -25,
	    '11100111': 26,
	    '111110010': -26,
            '111101110': -30,
	    '1111100110': 35,
	    '111100111': 36,
	    '1111101001': 40,
    }
    try:
        return decode(codebook, br)
    except:
        raise RuntimeError("Unknown X code")

def get_deltay(br):
    codebook = {
            '00000': 0,
            '00001': 1,
            '10001': -1,
            '00010': 2,
            '10000': -2,
            '00011': 3,
            '01111': -3,
            '00100': 4,
            '01110': -4,
            '00101': 5,
            '01101': -5,
            '00110': 6,
            '01100': -6,
            '00111': 7,
            '01011': -7,
            '01000': 8,
            '01010': -8,
            '100100': 9,
            '01001': -9,
            '100101': 10,
            '101101': -10,
            '100110': 11,
            '101100': -11,
            '100111': 12,
            '101011': -12,
            '101000': 13,
            '101010': -13,
            '1011100': 14,
            '101001': -14,
            '1011101': 15,
            '1101000': -15,
            '1011110': 16,
            '1100111': -16,
            '1011111': 17,
            '1100110': -17,
            '1100000': 18,
            '1100101': -18,
            '1100001': 19,
            '1100100': -19,
	    '1100010': 20,
	    '1100011': -20,
	    '11010010': 21,
	    '11100011': -21,
	    '11010011': 22,
	    '11100010': -22,
	    '11010100': 23,
	    '11100001': -23,
            '11010101': 24,
            '11100000': -24,
            '11010110': 25,
            '11011111': -25,
            '11010111': 26,
            '11011110': -26,
            '11011000': 27,
            '11011101': -27,
            '11011001': 28,
            '111100111': -28,
            '11011010': 29,
            '111100110': -29,
            '11011011': 30,
            '111100011': -32,
            '111100010': -33,
            '111001010': 34,
            '111001011': 35,
            '111100000': -35,
            '111001110': 38,
            '111010000': 40,
            '1111110011': -56,
            '1111110000': -59,
            '1111100110': 72,
    }
    try:
        return decode(codebook, br)
    except:
        raise RuntimeError("Unknown Y code")

def get_deltaforce(br):
    codebook = {
            '0': 0,
	    '1010': -1,
	    '10111': -2,
	    '110001': -3,
	    '1100100': 4,
	    '1100111': -4,
	    '11011111': -5,
	    '11011110': -6,
	    '11011101': -7,
	    '111100100': -8,
	    '111100011': -9,
	    '111100010': -10,
	    '111100001': -11,
	    '11011100': -12,
	    '11011011': -13,
	    '11011010': -14,
	    '11011001': -15,
	    '11011000': -16,
	    '11010111': -17,
	    '111100000': -18,
	    '111011111': -19,
	    '111011110': -20,
	    '111011101': -21,
	    '1111111100': -22,
	    '1111111011': -23,
	    '1111111010': -24,
	    '1111111001': -25,
	    '1111111000': -26,
	    '1111110111': -27,
	    '1111110110': -28,
	    '1111110101': -29,
            '11111111100': -30,
            '111000111': 30,
    }
    try:
        return 2*decode(codebook, br)
    except:
        raise RuntimeError("Unknown force code")


class STFParser(object):
    def __init__(self, stream):
        br = BitReader(stream)
        self.br = br
        magic = br.get_bits(16)
        if magic != 0x0100:
            raise RuntimeError("bad magic %x" % magic)

        br.get_bits(8*14)
        #version = stream.read(14)
        #if version != "Anoto STF v1.0":
        #    raise RuntimeError("bad version %s" % version)

        speed = br.get_bits(16)
        self.speed = speed

    def parse(self):
        start_time = 0
        br = self.br

        while True:
            br.flush()
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
                f0 = get_force(br)

                self.handle_point(x0, y0, f0, start_time)

                xa=0
                ya=0
                while True:
                    header = br.get_bits()

                    if header == 1:
                        trash = br.get_bits() # this header is 2 bits

                    time = get_time(br)
                    if time == 0:
                        self.handle_stroke_end(time)
                        break

                    do_delta = True
                    if header == 1:
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
                        deltax = get_deltax(br)
                        deltay = get_deltay(br)

                    deltaf = get_deltaforce(br)

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
