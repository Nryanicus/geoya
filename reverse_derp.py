points = []
with open("derp","r") as f:
    a = f.readline()
    while a:
        points.append(a)
        a = f.readline()

with open("derp","w") as f:
    while points:
        f.write(points.pop())
