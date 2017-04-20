import os,django
os.environ.setdefault('DJANGO_SETTINGS_MODULE','RatonGato.settings')
django.setup()
from server.models import User
from server.models import Game
from server.models import Move
    
#Compruebe si existe un usuario con id=10
# en caso contrario crear el usuario
# (XXXX.objects.get(id=10).exists())
id=10
username='u10'
password='p10'
u = User.objects.filter(id=id)
if u.exists():
    u10 = u[0]
    print("El usuario con id = %d ya existe"%id)
else:
    u10=User(id=id,password=password,username=username)
    u10.save()

id=11
username='u11'
password='p11'
u = User.objects.filter(id=id)
if u.exists():
    u11 = u[0]
    print("El usuario con id = %d ya existe"%id)
else:
    print("No existia un usuario con id = %d , creando..."%id)
    u11=User(id=id,password=password,username=username)
    u11.save()

print("Asignando catUser a usuario...")
g = Game(catUser = u10)
g.save()

print("Comprobando si existe algun juego sin raton")
g1 = Game.objects.filter(mouseUser__isnull=True)
if g1.exists():
    g = g1[0]
    print("hay un juego con el usuario raton vacio")
else:
    print("No hay un juego con el usuario raton vacio")

g.mouseUser = u11
g.save()

print("Ejecutando los movimientos pedidos...")
m1 = Move(origin = 59, target = 52, game = g)
m2 = Move(origin = 4, target = 11, game = g)

m1.save()
m2.save()

print("test completado correctamente.")


