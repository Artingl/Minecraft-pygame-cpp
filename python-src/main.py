import gc
import math
import os
import time
from random import randint
import pyglet
from OpenGL.GL import *
from OpenGL.raw.GLU import gluOrtho2D
import debug_module
from functions import drawInfoLabel, getElpsTime, translateSeed
from game.GUI.Button import Button
from game.GUI.Editarea import Editarea
from game.GUI.GUI import GUI
from game.GUI.Sliderbox import Sliderbox
from game.entity.Player import Player
from game.sound.BlockSound import BlockSound
from game.sound.Sound import Sound
from game.Scene import Scene
from settings import *
import opengl_main_cpp


def getWorlds():
    result = []
    for i in os.listdir("saves"):
        result.append((i, "Nope", 0, 1))
    return result

    #       ("New World", "New World (5) (4/26/21 7:39 PM)", 0, 1)
    #         ^ Name        ^ Folder name and time           ^ game mode (0 - survival) and is cheats on


def playSelected():
    global haveToGenWorld, chunks
    # haveToGenWorld = False
    lvl_name = worlds[selectedWorld][0]
    opengl_main_cpp.setLevelName(lvl_name)

    chunks = []
    for i in os.listdir("saves/" + lvl_name + "/chunks/"):
        with open("saves/" + lvl_name + "/chunks/" + i, "r") as file:
            content = file.read(os.path.getsize("saves/" + lvl_name + "/chunks/" + i))
        coords = content.split("\n")[0].split(" ")
        blocks = []
        for j in content.split("\n")[1].split(";"):
            if j:
                k = j.split(",")
                blocks.append([str(k[0]), int(k[1]), int(k[2]), int(k[3])])
        chunks.append(list(blocks))
    opengl_main_cpp.loadWorld(chunks)
    debug_module._gl_engine_info("mainInfoTest", "Yup!")
    startNewGame()


def quitTheGame():
    opengl_main_cpp.engineQuit()
    exit(0)


def respawn():
    pause()
    player.hp = 20
    player.isEntityDead = False
    scene.player.position = [
        opengl_main_cpp.getPlayerStartX(),
        opengl_main_cpp.getPlayerStartY(),
        opengl_main_cpp.getPlayerStartZ()
    ]


def quitToMenu():
    global PAUSE, IN_MENU, mainFunction

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    for x in range(0, scene.WIDTH, tex.width):
        for y in range(0, scene.HEIGHT, tex.height):
            tex.blit(x, y)
            tex2.blit(x, y)
    drawInfoLabel(scene, "Quitting to main menu", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2,
                  style=[('', '')], size=12, anchor_x='center')
    pygame.display.flip()
    clock.tick(MAX_FPS)

    PAUSE = True
    IN_MENU = True

    opengl_main_cpp.deleteWorld()
    time.sleep(1)
    sound.initMusic(False)

    sound.musicPlayer.play()
    sound.musicPlayer.set_volume(sound.volume)

    scene.resetScene()
    scene.initScene()

    player.position = [0, -90, 0]
    player.hp = 20
    player.isEntityDead = False

    mainFunction = drawMainMenu


def showSettings():
    global mainFunction
    mainFunction = drawSettingsMenu


def closeMenuWindows():
    global mainFunction
    mainFunction = drawMainMenu


def startNewGame():
    global mainFunction
    sound.musicPlayer.stop()
    sound.initMusic(True)
    opengl_main_cpp.setSeed(int(translateSeed(seedEditArea.text)))
    mainFunction = genWorld


def setWorldsMenu():
    global mainFunction, worlds, selectedWorld
    worlds = getWorlds()
    selectedWorld = -1
    mainFunction = drawWorldsMenu


def pause():
    global PAUSE, mainFunction
    PAUSE = not PAUSE
    scene.allowEvents["movePlayer"] = True
    scene.allowEvents["keyboardAndMouse"] = True
    mainFunction = pauseMenu


def deathScreen():
    global PAUSE, mainFunction
    PAUSE = not PAUSE
    scene.allowEvents["movePlayer"] = True
    scene.allowEvents["keyboardAndMouse"] = True
    mainFunction = drawDeathScreen


def drawSettingsMenu(mc):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(0, scene.HEIGHT, tex.height):
            tex.blit(ix, iy)
            tex2.blit(ix, iy)
    mp = pygame.mouse.get_pos()

    # Volume slider box
    soundVolumeSliderBox.x = scene.WIDTH // 2 - (soundVolumeSliderBox.bg.width // 2)
    soundVolumeSliderBox.y = scene.HEIGHT // 2 - (soundVolumeSliderBox.bg.height // 2) - 80
    soundVolumeSliderBox.update(mp)
    #

    # Render distance
    renderDistanceSliderBox.text = "Render Distance: " + str(renderDistanceSliderBox.val)
    renderDistanceSliderBox.x = scene.WIDTH // 2 - (renderDistanceSliderBox.bg.width // 2)
    renderDistanceSliderBox.y = scene.HEIGHT // 2 - (renderDistanceSliderBox.bg.height // 2)
    renderDistanceSliderBox.update(mp)
    scene.renderDistance = renderDistanceSliderBox.val
    opengl_main_cpp.setRenderDistance(int(renderDistanceSliderBox.val))
    #

    # Seed edit area
    seedEditArea.x = scene.WIDTH // 2 - (seedEditArea.bg.width // 2)
    seedEditArea.y = scene.HEIGHT // 2 - (seedEditArea.bg.height // 2) + 80
    seedEditArea.update(mp, mc, keys)
    #

    # Close
    closeSettingsButton.x = scene.WIDTH // 2 - (closeSettingsButton.button.width // 2)
    closeSettingsButton.y = scene.HEIGHT // 2 - (closeSettingsButton.button.height // 2) + 160
    closeSettingsButton.update(mp, mc)
    #

    sound.musicPlayer.set_volume(soundVolumeSliderBox.val / 100)
    sound.volume = soundVolumeSliderBox.val / 100

    pygame.display.flip()
    clock.tick(MAX_FPS)


def drawDeathScreen(mc):
    bg = gui.GUI_TEXTURES["red"]
    bg.width = scene.WIDTH
    bg.height = scene.HEIGHT
    bg.blit(0, 0)

    mp = pygame.mouse.get_pos()

    drawInfoLabel(scene, f"You died!", xx=scene.WIDTH // 2, yy=scene.HEIGHT - scene.HEIGHT // 4, style=[('', '')],
                  size=34, anchor_x='center')

    # Back to Game button
    respawnButton.x = scene.WIDTH // 2 - (respawnButton.button.width // 2)
    respawnButton.y = scene.HEIGHT // 2 - (respawnButton.button.height // 2) - 50
    respawnButton.update(mp, mc)
    #

    # Quit to title button
    quitWorldButton.text = "Title screen"
    quitWorldButton.x = scene.WIDTH // 2 - (quitButton.button.width // 2)
    quitWorldButton.y = scene.HEIGHT // 2 - (quitButton.button.height // 2)
    quitWorldButton.update(mp, mc)
    #

    pygame.display.flip()
    clock.tick(MAX_FPS)


def pauseMenu(mc):
    bg = gui.GUI_TEXTURES["black"]
    bg.width = scene.WIDTH
    bg.height = scene.HEIGHT
    bg.blit(0, 0)

    mp = pygame.mouse.get_pos()

    drawInfoLabel(scene, f"Game menu", xx=scene.WIDTH // 2, yy=scene.HEIGHT - scene.HEIGHT // 4, style=[('', '')],
                  size=12, anchor_x='center')

    # Back to Game button
    resumeButton.x = scene.WIDTH // 2 - (resumeButton.button.width // 2)
    resumeButton.y = scene.HEIGHT // 2 - (resumeButton.button.height // 2) - 50
    resumeButton.update(mp, mc)
    #

    # Quit to title button
    quitWorldButton.text = "Quit to title"
    quitWorldButton.x = scene.WIDTH // 2 - (quitButton.button.width // 2)
    quitWorldButton.y = scene.HEIGHT // 2 - (quitButton.button.height // 2)
    quitWorldButton.update(mp, mc)
    #

    pygame.display.flip()
    clock.tick(MAX_FPS)


def setCreateWorld():
    global mainFunction
    worldNameEditArea.text = "New World"
    mainFunction = createWorld


def createWorld(mc):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(0, scene.HEIGHT, tex.height):
            tex.blit(ix, iy)
            tex2.blit(ix, iy)

    createNewWorldButton.setEvent(startNewGame)
    cancelButton.setEvent(setWorldsMenu)

    mp = pygame.mouse.get_pos()

    drawInfoLabel(scene, "Create New World",
                  xx=scene.WIDTH // 2, yy=scene.HEIGHT - 80, style=[('', '')], anchor_x='center', size=12)

    # World name edit area
    worldNameEditArea.bg.width = 404
    worldNameEditArea.bg.height = 44
    worldNameEditArea.x = scene.WIDTH // 2 - (worldNameEditArea.bg.width // 2)
    worldNameEditArea.y = 140
    worldNameEditArea.update(mp, mc, keys)
    #

    drawInfoLabel(scene, "Will be saved in: " + (worldNameEditArea.text if worldNameEditArea.text else "World"),
                  xx=worldNameEditArea.x, yy=scene.HEIGHT - worldNameEditArea.y - 64, style=[('', '')],
                  size=12, label_color=(199, 199, 199))
    createNewWorldButton.active = bool(worldNameEditArea.text)

    # Game mode
    gameModeButton.x = scene.WIDTH // 2 - (gameModeButton.button.width // 2)
    gameModeButton.y = worldNameEditArea.y + 100
    gameModeButton.update(mp, mc)
    #

    drawInfoLabel(scene, "Only survival now available!",
                  xx=worldNameEditArea.x, yy=scene.HEIGHT - gameModeButton.y - 59, style=[('', '')],
                  size=12, label_color=(199, 199, 199))

    # More World Options...
    moreWorldOptionsButton.x = scene.WIDTH // 2 - (moreWorldOptionsButton.button.width // 2)
    moreWorldOptionsButton.y = gameModeButton.y + 105
    moreWorldOptionsButton.update(mp, mc)
    #

    # Create new world button
    createNewWorldButton.x = scene.WIDTH // 2 - (createNewWorldButton.button.width // 2) - (
                createNewWorldButton.button.width // 2) - 10
    createNewWorldButton.y = scene.HEIGHT - createNewWorldButton.button.height * 3 + 70
    createNewWorldButton.update(mp, mc)
    #

    # Cancel button
    cancelButton.x = scene.WIDTH // 2 - (cancelButton.button.width // 2) + (cancelButton.button.width // 2) + 10
    cancelButton.y = scene.HEIGHT - cancelButton.button.height * 3 + 70
    cancelButton.update(mp, mc)
    #

    # # Seed edit area
    # seedEditArea.x = scene.WIDTH // 2 - (seedEditArea.bg.width // 2)
    # seedEditArea.y = scene.HEIGHT // 2 - (seedEditArea.bg.height // 2)
    # seedEditArea.update(mp, mc, keys)
    # #

    pygame.display.flip()
    clock.tick(MAX_FPS)


def getNewLevelName():
    cnt = 0
    lvl_name = worldNameEditArea.text
    while os.path.isdir("saves/" + lvl_name):
        cnt += 1
        lvl_name = worldNameEditArea.text + " " + str(cnt)

    return lvl_name


def genWorld(mc):
    global IN_MENU, PAUSE, resizeEvent, chunks
    chunkCnt = 1  # 220

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(0, scene.HEIGHT, tex.height):
            tex.blit(ix, iy)
            tex2.blit(ix, iy)

    # scene.genWorld()
    # if scene.chunks.worldGen_chunkPosition[1] >= RENDER_DISTANCE * 2:
    #     IN_MENU = False
    #     PAUSE = False
    # return

    # proc = round((scene.chunks.worldGen_chunkPosition[1]) * 100 / (RENDER_DISTANCE * 2))
    # debug_module._gl_engine_info("INFO_PYTHON_MAIN_WORLD_GENERATOR", f"{proc}%/100%")
    # drawInfoLabel(scene, "Loading world...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2, style=[('', '')],
    #               size=12, anchor_x='center')
    # drawInfoLabel(scene, f"Generating terrain {proc}%...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2 - 39,
    #               style=[('', '')], size=12, anchor_x='center')

    # TODO
    if haveToGenWorld:
        # lvl_name = getNewLevelName()
        # opengl_main_cpp.setLevelName(lvl_name)
        # os.mkdir("saves/" + lvl_name)
        # os.mkdir("saves/" + lvl_name + "/chunks")
        # open("saves/" + lvl_name + "/world.data", "w").close()
        # open("saves/" + lvl_name + "/blocks.data", "w").close()

        drawInfoLabel(scene, "Loading world...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2, style=[('', '')],
                      size=12, anchor_x='center')
        drawInfoLabel(scene, "Generating world...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2 - 39,
                      style=[('', '')], size=12, anchor_x='center')
        pygame.display.flip()
        clock.tick(MAX_FPS)
        opengl_main_cpp.generateLevel(-128, -128, 128, 128, 64)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(0, scene.HEIGHT, tex.height):
            tex.blit(ix, iy)
            tex2.blit(ix, iy)

    drawInfoLabel(scene, "Loading world...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2, style=[('', '')],
                  size=12, anchor_x='center')
    drawInfoLabel(scene, "Generating chunks...", xx=scene.WIDTH // 2, yy=scene.HEIGHT // 2 - 39,
                  style=[('', '')], size=12, anchor_x='center')
    pygame.display.flip()
    clock.tick(MAX_FPS)
    opengl_main_cpp.generateChunks(0, 0, 0)

    IN_MENU = False
    PAUSE = False
    chunks = []

    scene.player.position = [
        opengl_main_cpp.getPlayerStartX(),
        opengl_main_cpp.getPlayerStartY(),
        opengl_main_cpp.getPlayerStartZ()
    ]
    # TODO


def drawWorldsMenu(mc):
    global selectedWorld
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    scene.set2d()

    tex = gui.GUI_TEXTURES["options_background"]
    tex2 = gui.GUI_TEXTURES["black"]
    tex3 = gui.GUI_TEXTURES["black2"]
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(0, scene.HEIGHT, tex.height):
            tex.blit(ix, iy)
            tex3.blit(ix, iy)
            tex2.blit(ix, iy)

    if selectedWorld != -1 and selectedWorld < len(worlds):
        yy = scene.HEIGHT - 144 - ((selectedWorld + 1) * 80)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
        t = gui.GUI_TEXTURES["edit_bg2"]
        sw = t.width
        sh = t.height
        t.width = 440
        t.height = 85
        t.blit(scene.WIDTH // 2 - 160, yy)
        t.width = sw
        t.height = sh

    y = scene.HEIGHT - 160
    for i in worlds:
        sett = ""
        if i[2] == 0:
            sett = "Survival, "
        if i[2] == 1:
            sett = "Creative, "
        if i[3] == 0:
            sett = sett[:-2]
        else:
            sett += "Cheats"

        drawInfoLabel(scene, i[0], xx=scene.WIDTH // 2 - 150, yy=y, style=[('', '')], size=12)
        drawInfoLabel(scene, i[1], xx=scene.WIDTH // 2 - 150, yy=y - 25, style=[('', '')], size=12,
                      label_color=(199, 199, 199))
        drawInfoLabel(scene, sett, xx=scene.WIDTH // 2 - 150, yy=y - 50, style=[('', '')], size=12,
                      label_color=(199, 199, 199))

        y -= 80

    for ix in range(0, scene.WIDTH, tex.width):
        tex.blit(ix, scene.HEIGHT - 65)
        tex2.blit(ix, scene.HEIGHT - 65)
    for ix in range(0, scene.WIDTH, tex.width):
        for iy in range(128, 0, -(128 // 2)):
            tex.blit(ix, iy - tex.height)
            tex2.blit(ix, iy - tex.height)

    mp = pygame.mouse.get_pos()

    if scene.WIDTH // 2 - 160 < mp[0] < scene.WIDTH // 2 - 160 + 440 and mc == 1:
        pos = mp[1] - 144
        if pos > 0:
            pos = pos // 85
            if pos < len(worlds):
                selectedWorld = pos

    drawInfoLabel(scene, "Select world", xx=scene.WIDTH // 2, yy=scene.HEIGHT - 49, style=[('', '')],
                  size=12, anchor_x='center')
    createNewWorldButton.active = True

    playSelectedWorldButton.active = selectedWorld != -1
    deleteWorldButton.active = selectedWorld != -1

    createNewWorldButton.setEvent(setCreateWorld)
    cancelButton.setEvent(closeMenuWindows)

    # Play selected world button
    playSelectedWorldButton.x = scene.WIDTH // 2 - (playSelectedWorldButton.button.width // 2) - (
                playSelectedWorldButton.button.width // 2) - 10
    playSelectedWorldButton.y = scene.HEIGHT - playSelectedWorldButton.button.height * 3 + 15
    playSelectedWorldButton.update(mp, mc)
    #

    # Create new world button
    createNewWorldButton.x = scene.WIDTH // 2 - (createNewWorldButton.button.width // 2) + (
                createNewWorldButton.button.width // 2) + 10
    createNewWorldButton.y = scene.HEIGHT - createNewWorldButton.button.height * 3 + 15
    createNewWorldButton.update(mp, mc)
    #

    # Cancel button
    cancelButton.x = scene.WIDTH // 2 - (cancelButton.button.width // 2) + (cancelButton.button.width // 2) + 10
    cancelButton.y = scene.HEIGHT - cancelButton.button.height * 3 + 70
    cancelButton.update(mp, mc)
    #

    # Delete world button
    deleteWorldButton.x = scene.WIDTH // 2 - (deleteWorldButton.button.width // 2) - (
                deleteWorldButton.button.width // 2) - 10
    deleteWorldButton.y = scene.HEIGHT - deleteWorldButton.button.height * 3 + 70
    deleteWorldButton.update(mp, mc)
    #

    pygame.display.flip()
    clock.tick(MAX_FPS)


def drawMainMenu(mc):
    global mainMenuRotation, IN_MENU, PAUSE
    glFogfv(GL_FOG_COLOR, (GLfloat * 4)(0.5, 0.7, 1, 1))
    glFogf(GL_FOG_START, 0)
    glFogf(GL_FOG_END, 1000)

    scene.set3d()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()

    glPushMatrix()
    glRotatef(mainMenuRotation[0], 1, 0, 0)
    glRotatef(mainMenuRotation[1], 0, 1, 0)
    glTranslatef(0, 0, 0)

    scene.stuffBatch.draw()
    scene.stuffBatch = pyglet.graphics.Batch()
    scene.drawPanorama()

    glPopMatrix()
    scene.set2d()
    mp = pygame.mouse.get_pos()

    tex = gui.GUI_TEXTURES["game_logo"]
    tex.blit(scene.WIDTH // 2 - (tex.width // 2), scene.HEIGHT - tex.height - (scene.HEIGHT // 15))

    drawInfoLabel(scene, f"Minecraft {MC_VERSION} (Pygame edition)", xx=10, yy=10, style=[('', '')], size=12)

    # Singleplayer button
    singleplayerButton.x = scene.WIDTH // 2 - (singleplayerButton.button.width // 2)
    singleplayerButton.y = scene.HEIGHT // 2 - (singleplayerButton.button.height // 2) - 25
    singleplayerButton.update(mp, mc)
    #

    # Options button
    optionsButton.x = scene.WIDTH // 2 - (optionsButton.button.width // 2)
    optionsButton.y = scene.HEIGHT // 2 - (optionsButton.button.height // 2) + 25
    optionsButton.update(mp, mc)
    #

    # Quit button
    quitButton.x = scene.WIDTH // 2 - (quitButton.button.width // 2)
    quitButton.y = scene.HEIGHT // 2 - (quitButton.button.height // 2) + 75
    quitButton.update(mp, mc)
    #

    # Splash
    glPushMatrix()
    glTranslatef((scene.WIDTH // 2 + (tex.width // 2)) - 90, scene.HEIGHT - tex.height - (scene.HEIGHT // 15) + 15, 0.0)
    glRotatef(20.0, 0.0, 0.0, 1.0)
    var8 = 1.8 - abs(math.sin((getElpsTime() % 1000) / 1000.0 * math.pi * 2.0) * 0.1)
    var8 = var8 * 100.0 / ((24 * 12) + 32)
    drawInfoLabel(scene, splash, xx=1, yy=1, style=[('', '')], scale=var8, size=30, anchor_x='center',
                  label_color=(255, 255, 0), shadow_color=(63, 63, 0))
    glPopMatrix()
    #

    pygame.display.flip()
    clock.tick(MAX_FPS)

    if mainMenuRotation[0] < 25:
        mainMenuRotation[2] = False
    if mainMenuRotation[0] > 75:
        mainMenuRotation[2] = True

    if mainMenuRotation[2]:
        mainMenuRotation[0] -= 0.02
    else:
        mainMenuRotation[0] += 0.02
    mainMenuRotation[1] += 0.02


debug_module._gl_engine_info("_main_python", "Loading the game...")

chunks = []
haveToGenWorld = True
resizeEvent = False
LAST_SAVED_RESOLUTION = [WIDTH, HEIGHT]
worlds = []

screen = pygame.display.set_mode((WIDTH, HEIGHT), pygame.DOUBLEBUF | pygame.OPENGL | pygame.RESIZABLE)
pygame.display.set_caption(f"Minecraft {MC_VERSION}")

# Loading screen
glClearColor(1, 1, 1, 1)

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
glLoadIdentity()

glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluOrtho2D(0, WIDTH, 0, HEIGHT)

logo = pyglet.resource.image("gui/logo.png")
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
logo.width = 318
logo.height = 318
logo.blit(WIDTH // 2 - (logo.width // 2), HEIGHT // 2 - (logo.height // 2))
pygame.display.flip()
#

sound = Sound()
scene = Scene()
scene.clock = clock
gui = GUI(scene)
blockSound = BlockSound(scene)
player = Player(gl=scene)

# player.position = [0, -9000, 0]
player.position = [0, 90, 0]

scene.blockSound = blockSound
scene.gui = gui
scene.sound = sound
scene.player = player

scene.deathScreen = deathScreen
scene.initScene()

debug_module._gl_engine_info("_main_python", "Loading sounds...")
sound.BLOCKS_SOUND["pickUp"] = pygame.mixer.Sound("sounds/pick.mp3")

debug_module._gl_engine_info("_main_python", "Loading step sounds...")
sound.BLOCKS_SOUND["step"] = {}
for e, i in enumerate(os.listdir("sounds/step/")):
    soundName = i.split(".")[0][:-1]
    soundNum = i.split(".")[0][-1]

    if soundName not in sound.BLOCKS_SOUND["step"]:
        sound.BLOCKS_SOUND["step"][soundName] = []

    sound.BLOCKS_SOUND["step"][soundName].append(pygame.mixer.Sound("sounds/step/" + i))
    debug_module._gl_engine_info("_main_python", "Successful loaded " + soundName + " #" + soundNum + " sound!")

debug_module._gl_engine_info("_main_python", "Loading dig sounds...")
sound.BLOCKS_SOUND["dig"] = {}
for e, i in enumerate(os.listdir("sounds/dig/")):
    soundName = i.split(".")[0][:-1]
    soundNum = i.split(".")[0][-1]

    if soundName not in sound.BLOCKS_SOUND["dig"]:
        sound.BLOCKS_SOUND["dig"][soundName] = []

    sound.BLOCKS_SOUND["dig"][soundName].append(pygame.mixer.Sound("sounds/dig/" + i))
    debug_module._gl_engine_info("_main_python", "Successful loaded " + soundName + " #" + soundNum + " sound!")

debug_module._gl_engine_info("_main_python", "Loading explode sounds...")
sound.BLOCKS_SOUND["explode"] = []
for e, i in enumerate(os.listdir("sounds/explode/")):
    soundName = i.split(".")[0][:-1]
    soundNum = i.split(".")[0][-1]

    sound.BLOCKS_SOUND["explode"].append(pygame.mixer.Sound("sounds/explode/" + i))
    debug_module._gl_engine_info("_main_python", "Successful loaded " + soundName + "#" + soundNum + " sound!")

debug_module._gl_engine_info("_main_python", "Loading damage sounds...")
sound.SOUNDS["damage"] = {}
for e, i in enumerate(os.listdir("sounds/damage/")):
    soundName = i.split(".")[0][:-1]
    soundNum = i.split(".")[0][-1]

    if soundName not in sound.SOUNDS["damage"]:
        sound.SOUNDS["damage"][soundName] = []

    sound.SOUNDS["damage"][soundName].append(pygame.mixer.Sound("sounds/damage/" + i))
    debug_module._gl_engine_info("_main_python", "Successful loaded " + soundName + " #" + soundNum + " sound!")

debug_module._gl_engine_info("_main_python", "Loading GUI sounds...")
sound.SOUNDS["GUI"] = {}
for e, i in enumerate(os.listdir("sounds/gui/")):
    soundName = i.split(".")[0][:-1]
    soundNum = i.split(".")[0][-1]

    if soundName not in sound.SOUNDS["GUI"]:
        sound.SOUNDS["GUI"][soundName] = []

    sound.SOUNDS["GUI"][soundName].append(pygame.mixer.Sound("sounds/gui/" + i))
    debug_module._gl_engine_info("_main_python", "Successful loaded " + soundName + " #" + soundNum + " sound!")

debug_module._gl_engine_info("_main_python", "Loading menu music...")
for e, i in enumerate(os.listdir("sounds/music/menu")):
    sound.MENU_MUSIC.append("sounds/music/menu/" + i)
    debug_module._gl_engine_info("_main_python", "Successful loaded " + i + " music!")

debug_module._gl_engine_info("_main_python", "Loading game music...")
for e, i in enumerate(os.listdir("sounds/music/game")):
    sound.MUSIC.append("sounds/music/game/" + i)
    debug_module._gl_engine_info("_main_python", "Successful loaded " + i + " music!")
sound.initMusic(False)

debug_module._gl_engine_info("_main_python", "Music loaded successful!")

debug_module._gl_engine_info("_main_python", "Loading GUI textures...")
gui.GUI_TEXTURES = {
    "crafting_table": pyglet.resource.image("gui/crafting_table.png"),
    "inventory_window": pyglet.resource.image("gui/inventory_window.png"),
    "crosshair": pyglet.resource.image("gui/crosshair.png"),
    "inventory": pyglet.resource.image("gui/inventory.png"),
    "sel_inventory": pyglet.resource.image("gui/sel_inventory.png"),
    "fullheart": pyglet.resource.image("gui/fullheart.png"),
    "halfheart": pyglet.resource.image("gui/halfheart.png"),
    "heartbg": pyglet.resource.image("gui/heartbg.png"),
    "strokeheartbg1": pyglet.resource.image("gui/strokeheartbg1.png"),
    "strokeheartbg2": pyglet.resource.image("gui/strokeheartbg2.png"),
    "game_logo": pyglet.resource.image("gui/game_logo.png"),
    "button_bg": pyglet.resource.image("gui/gui_elements/button_bg.png"),
    "button_bg_hover": pyglet.resource.image("gui/gui_elements/button_bg_hover.png"),
    "button_bg_hover_first6px": pyglet.resource.image("gui/gui_elements/button_bg_hover_first6px.png"),
    "button_bg_hover_last6px": pyglet.resource.image("gui/gui_elements/button_bg_hover_last6px.png"),
    "button_bg_first6px": pyglet.resource.image("gui/gui_elements/button_bg_first6px.png"),
    "button_bg_last6px": pyglet.resource.image("gui/gui_elements/button_bg_last6px.png"),
    "edit_bg_first6px": pyglet.resource.image("gui/gui_elements/edit_bg_first6px.png"),
    "edit_bg_last6px": pyglet.resource.image("gui/gui_elements/edit_bg_last6px.png"),
    "edit_bg": pyglet.resource.image("gui/gui_elements/edit_bg.png"),
    "edit_bg2": pyglet.resource.image("gui/gui_elements/edit_bg2.png"),
    "options_background": pyglet.resource.image("gui/gui_elements/options_background.png"),
    "black": pyglet.resource.image("gui/gui_elements/black.png"),
    "black2": pyglet.resource.image("gui/gui_elements/black2.png"),
    "red": pyglet.resource.image("gui/gui_elements/red.png"),
    "selected": pyglet.resource.image("gui/gui_elements/selected.png"),
    "slider": pyglet.resource.image("gui/gui_elements/slider.png"),
    "under_water": pyglet.resource.image("gui/under_water.png"),
}

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

for i, j in gui.GUI_TEXTURES.items():
    if i in ["options_background", "black", "black2", "red", "crosshair"]:
        continue
    texture = gui.GUI_TEXTURES[i]
    texture.width *= 2
    texture.height *= 2

texture = gui.GUI_TEXTURES["options_background"]
texture.width *= 6
texture.height *= 6

texture = gui.GUI_TEXTURES["black"]
texture.width *= 6
texture.height *= 6

texture = gui.GUI_TEXTURES["black2"]
texture.width *= 6
texture.height *= 6

texture = gui.GUI_TEXTURES["red"]
texture.width *= 6
texture.height *= 6

gui.addGuiElement("crosshair", (scene.WIDTH // 2 - 9, scene.HEIGHT // 2 - 9))
player.inventory.initWindow()

showInfoLabel = False
selectedWorld = -1

debug_module._gl_engine_info("_main_python", "Loading splashes...")
splfile = open("gui/splashes.txt", "r", encoding='utf-8')
splash = splfile.read().split("\n")
splash = splash[randint(0, len(splash) - 1)]
splfile.close()

sound.musicPlayer.play()
sound.musicPlayer.set_volume(sound.volume)

# Main menu buttons
singleplayerButton = Button(scene, "Singleplayer", 0, 0)
optionsButton = Button(scene, "Options", 0, 0)
quitButton = Button(scene, "Quit game", 0, 0)

singleplayerButton.setEvent(startNewGame)  # setWorldsMenu)
optionsButton.setEvent(showSettings)
quitButton.setEvent(quitTheGame)
#

# Settings objects
closeSettingsButton = Button(scene, "Done", 0, 0)
soundVolumeSliderBox = Sliderbox(scene, "Sound volume:", 100, 0, 0)
renderDistanceSliderBox = Sliderbox(scene, "Render Distance: 6", 12, 0, 0)
seedEditArea = Editarea(scene, "World seed", 0, 0)

renderDistanceSliderBox.minval = 2
renderDistanceSliderBox.val = RENDER_DISTANCE
closeSettingsButton.setEvent(closeMenuWindows)
#

# Worlds objects
playSelectedWorldButton = Button(scene, "Play Selected World", 0, 0, active=False)
createNewWorldButton = Button(scene, "Create New World", 0, 0)
deleteWorldButton = Button(scene, "Delete", 0, 0, active=False)
cancelButton = Button(scene, "Cancel", 0, 0)

playSelectedWorldButton.setEvent(playSelected)
createNewWorldButton.setEvent(setCreateWorld)
cancelButton.setEvent(closeMenuWindows)
#

# Create new world objects
worldNameEditArea = Editarea(scene, "World Name", 0, 0)
gameModeButton = Button(scene, "Game Mode: Survival", 0, 0, active=False)
moreWorldOptionsButton = Button(scene, "More World Options...", 0, 0, active=False)

worldNameEditArea.text = "New World"
#

# Pause menu buttons
resumeButton = Button(scene, "Back to Game", 0, 0)
quitWorldButton = Button(scene, "Quit to title", 0, 0)

resumeButton.setEvent(pause)
quitWorldButton.setEvent(quitToMenu)
#

# Death screen buttons
respawnButton = Button(scene, "Respawn", 0, 0)
respawnButton.setEvent(respawn)
#

debug_module._gl_engine_info("_main_python", "Loading complete!")
mainMenuRotation = [50, 180, True]

mainFunction = drawMainMenu

if not os.path.isdir("saves"):
    os.mkdir("saves")

while True:
    if scene.allowEvents["keyboardAndMouse"] and not PAUSE:
        if pygame.mouse.get_pressed(3)[0]:
            player.mouseEvent(1)
    mbclicked = None
    keys = []

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            quitTheGame()
        if event.type == pygame.KEYDOWN:
            keys.append(event.key)
            if event.key == pygame.K_F11:
                if scene.WIDTH != monitor.current_w or scene.HEIGHT != monitor.current_h:
                    LAST_SAVED_RESOLUTION = [scene.WIDTH, scene.HEIGHT]

                    WIDTH = monitor.current_w
                    HEIGHT = monitor.current_h
                    screen = pygame.display.set_mode((monitor.current_w, monitor.current_h),
                                                     pygame.DOUBLEBUF | pygame.OPENGL | pygame.RESIZABLE
                                                     | pygame.FULLSCREEN)
                    scene.resizeCGL(WIDTH, HEIGHT)
                    resizeEvent = True
                else:
                    WIDTH = LAST_SAVED_RESOLUTION[0]
                    HEIGHT = LAST_SAVED_RESOLUTION[1]
                    screen = pygame.display.set_mode((WIDTH, HEIGHT),
                                                     pygame.DOUBLEBUF | pygame.OPENGL | pygame.RESIZABLE)
                    scene.resizeCGL(WIDTH, HEIGHT)
                    resizeEvent = True
        if event.type == pygame.VIDEORESIZE:
            WIDTH = event.w
            HEIGHT = event.h
            scene.resizeCGL(WIDTH, HEIGHT)
            resizeEvent = True
        if event.type == pygame.MOUSEBUTTONDOWN:
            mbclicked = event.button
        if not IN_MENU:
            if scene.allowEvents["keyboardAndMouse"]:
                if event.type == pygame.KEYDOWN:
                    debug_module._gl_engine_info("main_python_EVENT_HANDLER", "Key press: " + str(event.key))
                    if event.key == pygame.K_ESCAPE:
                        pause()
                    if event.key == pygame.K_e:
                        player.inventory.showWindow()
                    if event.key == pygame.K_1:
                        player.inventory.activeInventory = 0
                    if event.key == pygame.K_2:
                        player.inventory.activeInventory = 1
                    if event.key == pygame.K_3:
                        player.inventory.activeInventory = 2
                    if event.key == pygame.K_4:
                        player.inventory.activeInventory = 3
                    if event.key == pygame.K_5:
                        player.inventory.activeInventory = 4
                    if event.key == pygame.K_6:
                        player.inventory.activeInventory = 5
                    if event.key == pygame.K_7:
                        player.inventory.activeInventory = 6
                    if event.key == pygame.K_8:
                        player.inventory.activeInventory = 7
                    if event.key == pygame.K_9:
                        player.inventory.activeInventory = 8
                    if event.key == pygame.K_F3:
                        showInfoLabel = not showInfoLabel
                    if event.key == pygame.K_F5:
                        player.cameraType += 1
                        if player.cameraType > 3:
                            player.cameraType = 1
                if event.type == pygame.MOUSEBUTTONDOWN:
                    player.mouseEvent(event.button)
                    if event.button == 4:
                        player.inventory.activeInventory -= 1
                        if player.inventory.activeInventory < 0:
                            player.inventory.activeInventory = 8
                        if player.inventory.inventory[player.inventory.activeInventory][1]:
                            gui.showText(player.inventory.inventory[player.inventory.activeInventory][0])
                    elif event.button == 5:
                        player.inventory.activeInventory += 1
                        if player.inventory.activeInventory > 8:
                            player.inventory.activeInventory = 0
                        if player.inventory.inventory[player.inventory.activeInventory][1]:
                            gui.showText(player.inventory.inventory[player.inventory.activeInventory][0])
                else:
                    if pygame.mouse.get_pressed(3)[0]:
                        player.mouseEvent(1)
                    else:
                        player.mouseEvent(-1)
        else:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    mainFunction = drawMainMenu

    if scene.allowEvents["grabMouse"]:
        pygame.mouse.set_visible(PAUSE)
    else:
        pygame.mouse.set_visible(True)

    if IN_MENU:
        mainFunction(mbclicked)

    if not PAUSE:
        sound.playMusic()

        if scene.allowEvents["showCrosshair"]:
            gui.shows["crosshair"][1] = (scene.WIDTH // 2 - 9, scene.HEIGHT // 2 - 9)
        else:
            gui.shows["crosshair"][1] = (-100, -100)
        if scene.allowEvents["grabMouse"] and pygame.mouse.get_focused():
            pygame.mouse.set_pos((scene.WIDTH // 2, scene.HEIGHT // 2))
        scene.updateScene()

        player.inventory.draw()
        gui.update()

        # if showInfoLabel:
        #     drawInfoLabel(scene, f"Minecraft {MC_VERSION} ({MC_VERSION}/vanilla)\n"
        #                          f"{round(clock.get_fps())} fps\n"
        #                          f"\n"
        #                          f"XYZ: {round(player.x(), 3)} / {round(player.y(), 5)} / {round(player.z(), 3)}\n"
        #                          f"Block: {round(player.x())} / {round(player.y())} / {round(player.z())}\n"
        #                          f"Facing: {round(player.rotation[1], 3)} / {round(player.rotation[0], 3)}\n"
        #     # f"Biome: {getBiomeByTemp(scene.worldGen.perlinBiomes(player.x(), player.z()) * 3)}\n"
        #                          f"Looking at: {scene.lookingAt}\n",
        #                   shadow=False, label_color=(224, 224, 224), xx=3)
        pygame.display.flip()
        clock.tick(MAX_FPS)
    elif PAUSE and not IN_MENU:
        scene.allowEvents["movePlayer"] = False
        scene.allowEvents["keyboardAndMouse"] = False
        if scene.allowEvents["showCrosshair"]:
            gui.shows["crosshair"][1] = (scene.WIDTH // 2 - 9, scene.HEIGHT // 2 - 9)
        else:
            gui.shows["crosshair"][1] = (-100, -100)
        scene.updateScene()

        player.inventory.draw()
        gui.update()

        mainFunction(mbclicked)
