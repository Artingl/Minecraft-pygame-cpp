import os
import time
from random import randint

import numpy
import pyglet
from OpenGL.GL import *
from PIL import Image
import debug_module
from settings import *


def genPos(_id, w, h):
    x, y = 0, 0
    if _id == 0:
        return x, y

    for i in range(_id):
        x += 16
        if x > w:
            x = 0
            y += 16

    return x, y


def load_textures(self):
    debug_module._gl_engine_info("_functions_python", "Loading textures...")
    t = self.texture
    dirs = ['textures', 'textures/terrain']
    size = (1600, 1600)
    allTextures = pygame.surface.Surface(size)
    cnt = 0
    while dirs:
        d = dirs.pop(0)
        textures = list(os.listdir(d))
        for e, file in enumerate(textures):
            if file == "invalid.png":
                textures[0], textures[e] = textures[e], textures[0]
                break
        for file in textures:
            if os.path.isdir(d + '/' + file):
                dirs += [d + '/' + file]
            else:
                if ".png" not in file or d == "textures/blocks/block_destroy":
                    continue

                n = file.split('.')[0]
                path = d + '/' + file

                if d == 'textures/terrain':
                    image = pyglet.image.load(d + '/' + file)
                    texture = image.get_mipmapped_texture()
                    self.terrain[n] = pyglet.graphics.TextureGroup(texture)
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
                    continue
                img = pygame.image.load(path)
                allTextures.blit(img, genPos(cnt, *size))

                if d == "textures/blocks/tbs":
                    if n[0:-1].strip() not in self.block:
                        self.block[n[0:-1].strip()] = {}

                    if n.endswith('t'):
                        self.block[n[0:-1].strip()][0] = cnt
                    if n.endswith('b'):
                        self.block[n[0:-1].strip()][1] = cnt
                    if n.endswith('s'):
                        self.block[n[0:-1].strip()][2] = cnt
                elif d == "textures/blocks/ts":
                    if n[0:-1].strip() not in self.block:
                        self.block[n[0:-1].strip()] = {}

                    if n.endswith('t'):
                        self.block[n[0:-1].strip()][0] = cnt
                        self.block[n[0:-1].strip()][1] = cnt
                    if n.endswith('s'):
                        self.block[n[0:-1].strip()][2] = cnt
                else:
                    self.block[n] = (cnt, cnt, cnt)

                debug_module._gl_engine_info("_functions_python", "Successful loaded " + n + " texture!")
                cnt += 1
    pygame.image.save(allTextures, "test.png")
    width = allTextures.get_width()
    height = allTextures.get_height()
    bgImgGL = glGenTextures(1)
    glBindTexture(GL_TEXTURE_2D, bgImgGL)
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pygame.image.tostring(allTextures, "RGB"))
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glGenerateMipmap(GL_TEXTURE_2D)
    self.texture["world"] = bgImgGL
    debug_module._gl_engine_info("_functions_python", "Successful loaded " + str(cnt) + " textures!")


def translateSeed(seed):
    res = ""
    if seed == "":
        seed = str(randint(998, 43433))
    for i in seed:
        res += str(ord(i))
    while len(res) < 10:
        res += res[:-1]
    return int(res[0:10])


def cube_vertices(pos, n=0.5):
    x, y, z = pos
    v = tuple((x + X, y + Y, z + Z) for X in (-n, n) for Y in (-n, n) for Z in (-n, n))
    return tuple(tuple(k for j in i for k in v[j]) for i in
                 ((0, 1, 3, 2), (5, 4, 6, 7), (0, 4, 5, 1), (3, 7, 6, 2), (4, 0, 2, 6), (1, 5, 7, 3)))


def flatten(lst): return sum(map(list, lst), [])


def roundPos(pos):
    x, y, z = pos
    return round(x), round(y), round(z)


def getSum(s):
    res = 0
    for i in s:
        res += int(i)

    return res


def drawInfoLabel(gl, text, xx=0, yy=0, style=None, size=15, anchor_x='left', anchor_y='baseline', opacity=1, rotate=0,
                  label_color=(255, 255, 255), shadow_color=(56, 56, 56), scale=0, shadow=True):
    if style is None:
        style = []
    y = -21
    ms = size / 6
    for i in text.split("\n"):
        ix = ms
        iy = gl.HEIGHT + y + yy - ms
        if xx:
            ix = xx + ms
        if yy:
            iy = yy - ms
        shadow_lbl = pyglet.text.Label(i,
                                       font_name='Minecraft Rus',
                                       color=(shadow_color[0], shadow_color[1], shadow_color[2], round(opacity * 255)),
                                       font_size=size,
                                       x=ix, y=iy,
                                       anchor_x=anchor_x,
                                       anchor_y=anchor_y)
        lbl = pyglet.text.Label(i,
                                font_name='Minecraft Rus',
                                color=(label_color[0], label_color[1], label_color[2], round(opacity * 255)),
                                font_size=size,
                                x=ix - ms, y=iy + ms,
                                anchor_x=anchor_x,
                                anchor_y=anchor_y)
        if not style:
            if shadow:
                shadow_lbl.set_style("background_color", (69, 69, 69, 100))
            else:
                lbl.set_style("background_color", (69, 69, 69, 100))
        else:
            for st in style:
                lbl.set_style(st[0], st[1])
                shadow_lbl.set_style(st[0], st[1])
        if rotate:
            glRotatef(rotate, 0.0, 0.0, 1.0)
        if scale:
            glScalef(scale, scale, 0)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        if shadow:
            shadow_lbl.draw()
        lbl.draw()
        if rotate:
            glRotatef(-rotate, 0.0, 0.0, 1.0)
        y -= 21


def getElpsTime():
    return time.perf_counter_ns() * 1000 / 1000000000


def checkHover(ox, oy, ow, oh, mx, my):
    if ox < mx < ox + ow and oy < my < oy + oh:
        return True
    return False
