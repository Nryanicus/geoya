#!/usr/bin/python
import bpy
import os

scene = bpy.context.scene

NUM_FRAMES = 5
# ACTIONS = ("Air",  "Death",  "Life",  "Earth",  "Fire",  "Water",  "0Close", "1Close", "2Close", "3Close", "4Close", "Rotate", "Horz", "Vert", "Cast")
ACTIONS = ["1Close"]
CAMERAS = []
for ob in scene.objects:
    if ob.type == 'CAMERA':
        CAMERAS.append(ob)
        bpy.context.scene.objects.active = ob

armature = scene.objects["HandArma"]
bpy.context.scene.objects.active = armature
bpy.context.object.data.pose_position = 'POSE'

for action in ACTIONS:
    # set animation
    bpy.context.active_object.animation_data.action = bpy.data.actions[action]
    for camera in CAMERAS:
        bpy.context.scene.camera = camera
        for i in range(1, NUM_FRAMES+1):
            bpy.context.scene.frame_current = i

            filename = os.path.join("/home/nathan/Projects/GeoyaRedux/BlenderRender/", camera.name+action+str(i))
            bpy.context.scene.render.filepath = filename
            bpy.ops.render.render( write_still=True )
