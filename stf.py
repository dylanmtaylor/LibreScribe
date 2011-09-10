#!/usr/bin/python

import cairo
import sys
import parsestf

surface = cairo.ImageSurface(cairo.FORMAT_RGB24, 8000, 8000)
ctx = cairo.Context(surface)

ctx.set_source_rgb(255, 255, 255)
ctx.paint()
ctx.set_source_rgb(0,0,0)

class Parser(parsestf.STFParser):
    def __init__(self, *args):
        super(Parser, self).__init__(*args)
        self.last_force=0

    def handle_stroke_end(self, time):
        ctx.stroke()
        self.last_force = 0

    def handle_point(self, x, y, f, time):
#	print "handling point (x: " + str(x) + ", y: " + str(y) + ", f: " + str(f) + ")" 
        if f:
            if self.last_force:
                ctx.line_to(x, y)
            else:
                ctx.move_to(x, y)
        self.last_force = 1

try:
    stf_file
except NameError:
    stf_file = sys.argv[1]

try:
    png_file
except NameError:
    png_file = sys.argv[2]

f = file(stf_file)
p = Parser(f)
p.parse()

ctx.stroke()
surface.write_to_png(png_file)
