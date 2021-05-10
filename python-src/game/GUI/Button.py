import pyglet.image

from functions import *
from settings import *


class Button:
    def __init__(self, gl, text, x, y, active=True, w=400, h=40):
        self.text = text
        self.x = x
        self.y = y
        self.gl = gl
        self.event = None
        self.active = active
        self.width = 100
        self.height = 40

        self.button = gl.gui.GUI_TEXTURES["button_bg"]

    def setEvent(self, event):
        self.event = event

    def updateButtonSize(self):
        pass  # if self.width != self.button.width or self.height != self.button.height:
        #     f6 = None
        #     l6 = None
        #     if self.active:
        #         f6 = self.gl.gui.GUI_TEXTURES["button_bg_first6px"].get_image_data()
        #         l6 = self.gl.gui.GUI_TEXTURES["button_bg_last6px"].get_image_data()
        #     else:
        #         f6 = self.gl.gui.GUI_TEXTURES["edit_bg_first6px"].get_image_data()
        #         l6 = self.gl.gui.GUI_TEXTURES["edit_bg_last6px"].get_image_data()
        #     if f6 and l6 and self.button:
        #         i = self.button.get_region(0, 0, self.button.width, self.button.height)
        #         img = pyglet.image.create(self.width, self.height)
        #         img.blit_into(i.get_region(6, 0, self.width, self.height).get_image_data(), 0, 0, 0)
        #         img.blit_into(f6.get_image_data(), 0, 0, 0)
        #         # self.button.blit_into(l6.get_image_data(), self.width - 6, 0, 0)
        #         self.button = img

    def update(self, mp, mc):
        lbl_color = (255, 255, 255)
        if self.active:
            self.button = self.gl.gui.GUI_TEXTURES["button_bg"]
        else:
            self.button = self.gl.gui.GUI_TEXTURES["edit_bg"]
            lbl_color = (180, 180, 180)
        # self.button = self.button.get_region(0, 0, self.button.width, self.button.height)
        self.updateButtonSize()

        if checkHover(self.x, self.y,
                      self.button.width, self.button.height,
                      mp[0], mp[1]) and self.active:
            self.button = self.gl.gui.GUI_TEXTURES["button_bg_hover"]
            # self.button = self.button.get_region(0, 0, self.button.width, self.button.height)
            self.updateButtonSize()
            if mc == 1:
                self.gl.sound.playGuiSound("click")
                if self.event:
                    self.event()

        test = pyglet.text.Label(self.text, font_name='Minecraft Rus', font_size=12)

        self.button.blit(self.x, self.gl.HEIGHT - self.y - self.button.height)
        labelX = self.x + (self.button.width // 2) + (test.content_width // 2) - (test.content_width // 2)
        drawInfoLabel(self.gl, self.text, xx=labelX, yy=self.gl.HEIGHT - self.y - 25, style=[('', '')],
                      size=12, anchor_x='center', label_color=lbl_color)
