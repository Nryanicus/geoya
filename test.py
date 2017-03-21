from sfml import sf

class Hand(sf.Sprite):

    def __init__(self, texture):
        self.texture = texture




def main():
    # create the main window
    window = sf.RenderWindow(sf.VideoMode(1000, 1000), "Gestures")


    # start the game loop
    while window.is_open:
       # process events
       for event in window.events:
          # close window: exit
          if type(event) is sf.Event.CLOSED:
             window.close()

       window.clear() # clear screen
       window.display() # update the window

if __name__ == "__main__":
    main()