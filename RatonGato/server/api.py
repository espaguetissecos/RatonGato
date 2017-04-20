from tastypie.resources import ModelResource, ALL
from models import User, Game, Move # clase donde se almacenan los usuarios del sistema
from django.conf.urls import url
from tastypie.utils import trailing_slash

class UserResource(ModelResource):
    """good for search http://127.0.0.1:8000/server/api/user/?id=111&format=json"""

    class Meta:
        queryset = User.objects.all()
        # Este resorce -conjunto de webservices- se identifica con la etiqueta user
        resource_name = 'user'
        #las siguientes lineas no son relevantes para nuestro juego
        #informan al servidor de que autorice
        #busquedas tanto por id como por username
        filtering = {
            'id': ALL,
            'userName': ALL
        }

        #agnade al mapeo de urls los webservices que desarrolleis
    def prepend_urls(self):
        return [
            url(r"^(%s)/add_user%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('add_user'), name="api_add_user"),
            url(r"^(%s)/counter%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('counter'), name="api_counter"),
            url(r"^(%s)/login_user%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('login_user'), name="api_login_user"),
            url(r"^(%s)/add_game%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('add_game'), name="api_add_game"),
            url(r"^(%s)/clean_orphan_games%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('clean_orphan_games'), name="api_clean_orphan_games"),
            url(r"^(%s)/join_game%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('join_game'), name="api_join_game"),
            url(r"^(%s)/status%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('status'), name="api_status"),
            url(r"^(%s)/cat_move%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('cat_move'), name="api_cat_move"),
            url(r"^(%s)/mouse_move%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('mouse_move'), name="api_mouse_move"),
            url(r"^(%s)/move%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('move'), name="api_move"),
            url(r"^(%s)/logout%s$" % (self._meta.resource_name, trailing_slash()),
                self.wrap_view('logout'), name="api_logout"),
        ]

    def add_user(self, request, *args, **kwargs):

        statsDict = {}
        username = request.POST['username']
        password = request.POST['password']
        try:
            #crea un usuario nuevo
            u = User(password=password, username=username)
            #persistilo en la base de datos
            u.save()
            #rellena el diccionario que sera devuelto a la aplicacion
            #seria mas comodo devolver el usuario en lugar del id
            #pero la serializacion de objetos no suele funcionar bien
            statsDict['value'] = u.id
            statsDict['result'] = True
            statsDict['message'] = "userId"
        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False
        #guardar el identificador de usuario en una variable de session
        #request.session['userId'] =u.id
        #recuperar el identificador de usuario de una variable de sesion
        #id = request.session['userId']
        #recordad que para usar las variable de session hace falta
        #activas las cookies como se muestra en el script test_server.py
        #comprobar si variable 'counter' esta guardada en session
        #if 'counter' in request.session:
        return self.create_response(request, statsDict)

    def counter(self, request, *args, **kwargs):

        statsDict = {}
        if 'counter' in request.session:
            contador = request.session['counter']
            contador = contador + 1
            request.session['counter'] = contador

            statsDict['value'] = contador
            statsDict['result'] = True
            statsDict['message'] = "Contador"

        else:
            request.session['counter'] = 1
            statsDict['value'] = 1
            statsDict['result'] = True
            statsDict['message'] = "Sesion nueva"


        return self.create_response(request, statsDict)

    def login_user(self, request, *args, **kwargs):
        """para probar el web service podeis ejecutar cualquiera de los comandos siguientes"""
        """http://127.0.0.1:8000/server/api/v1/user/add_user/?username=perico&password=felices&format=json"""
        """curl  -i -d "username=u1&password=p1"  http://127.0.0.1:8000/server/api/v1/user/add_user/"""
        statsDict = {}
        username = request.POST['username']
        password = request.POST['password']
        try:
            user = User.objects.filter(username=username, password=password)

            if user.exists():
                u = user[0]
                request.session['userId'] = u.id

                statsDict['value'] = u.id
                statsDict['result'] = True
                statsDict['message'] = "userId"
            else:
                statsDict['message'] = "NoUser"
                statsDict['value'] = -1
                statsDict['result'] = False

            #rellena el diccionario que sera devuelto a la aplicacion
            #seria mas comodo devolver el usuario en lugar del id
            #pero la serializacion de objetos no suele funcionar bien
        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False

        #guardar el identificador de usuario en una variable de session
        #recuperar el identificador de usuario de una variable de sesion
        #recordad que para usar las variable de session hace falta
        #activas las cookies como se muestra en el script test_server.py
        #comprobar si variable 'counter' esta guardada en session
        #if 'counter' in request.session:

        return self.create_response(request, statsDict)
        #PREGUNTAR SI GUARDAR FUERA DE TRY EN SESISON

    def logout(self, request, *args, **kwargs):
        """para probar el web service podeis ejecutar cualquiera de los comandos siguientes"""
        """http://127.0.0.1:8000/server/api/v1/user/add_user/?username=perico&password=felices&format=json"""
        """curl  -i -d "username=u1&password=p1"  http://127.0.0.1:8000/server/api/v1/user/add_user/"""
        statsDict = {}
        if 'userId' in request.session:
            del request.session['userId']
        if 'gameId' in request.session:
            del request.session['gameId']
        if 'counter' in request.session:
            del request.session['counter']
        statsDict['value'] = -1
        statsDict['result'] = True
        statsDict['message'] = "Has cerrado sesion"
        return self.create_response(request, statsDict)

    def add_game(self, request, *args, **kwargs):
        """para probar el web service podeis ejecutar cualquiera de los comandos siguientes"""
        """http://127.0.0.1:8000/server/api/v1/user/add_user/?username=perico&password=felices&format=json"""
        """curl  -i -d "username=u1&password=p1"  http://127.0.0.1:8000/server/api/v1/user/add_user/"""
        statsDict = {}
        id = request.session['userId']
        try:
            if User.objects.filter(id = id).exists():
                u = User.objects.filter(id = id)
                g = Game(catUser=u[0])
                #persistilo en la base de datos
                g.save()
                #rellena el diccionario que sera devuelto a la aplicacion
                #seria mas comodo devolver el usuario en lugar del id
                #pero la serializacion de objetos no suele funcionar bien
                request.session['gameId']= g.id
                id_game = request.session['gameId']
                statsDict['value'] = id_game
                statsDict['result'] = True
                statsDict['message'] = "gameId"
        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False

        return self.create_response(request, statsDict)

    def clean_orphan_games(self, request, *args, **kwargs):
        """para probar el web service podeis ejecutar cualquiera de los comandos siguientes"""
        """http://127.0.0.1:8000/server/api/v1/user/add_user/?username=perico&password=felices&format=json"""
        """curl  -i -d "username=u1&password=p1"  http://127.0.0.1:8000/server/api/v1/user/add_user/"""
        statsDict = {}
        try:
            #crea un usuario nuevo
            if Game.objects.filter(mouseUser__isnull=True).exists():
                g = Game.objects.filter(mouseUser__isnull=True)
                n = len(g)
                for i in range(0, n):
                    g[i].delete()
                #persistilo en la base de datos
                #rellena el diccionario que sera devuelto a la aplicacion
                #seria mas comodo devolver el usuario en lugar del id
                #pero la serializacion de objetos no suele funcionar bien
                statsDict['value'] = n
                statsDict['result'] = True
                statsDict['message'] = "Borrados"
            else:
                statsDict['value'] = -1
                statsDict['result'] = False
                statsDict['message'] = "Sin juegos huerfanos"
        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False
        #guardar el identificador de usuario en una variable de session
        #request.session['userId'] =u.id
        #recuperar el identificador de usuario de una variable de sesion
        #id = request.session['userId']
        #recordad que para usar las variable de session hace falta
        #activas las cookies como se muestra en el script test_server.py
        #comprobar si variable 'counter' esta guardada en session
        #if 'counter' in request.session:
        return self.create_response(request, statsDict)

    def join_game(self, request, *args, **kwargs):
        """para probar el web service podeis ejecutar cualquiera de los comandos siguientes"""
        """http://127.0.0.1:8000/server/api/v1/user/add_user/?username=perico&password=felices&format=json"""
        """curl  -i -d "username=u1&password=p1"  http://127.0.0.1:8000/server/api/v1/user/add_user/"""
        statsDict = {}
        id = request.session["userId"]
        try:
            #crea un usuario nuevo
            if Game.objects.filter(mouseUser__isnull=True).exists():
                g = Game.objects.filter(mouseUser__isnull=True)
                u = User.objects.filter(id=id)
                game = g[0]
                game.mouseUser = u[0]
                game.save()
                request.session['gameId']= game.id
                id_game = request.session['gameId']
                #persistilo en la base de datos
                #rellena el diccionario que sera devuelto a la aplicacion
                #seria mas comodo devolver el usuario en lugar del id
                #pero la serializacion de objetos no suele funcionar bien
                statsDict['value'] = id_game
                statsDict['result'] = True
                statsDict['message'] = "Juego unido"
            else:
                statsDict['value'] = -1
                statsDict['result'] = False
                statsDict['message'] = "Sin juegos huerfanos"
        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False
        #guardar el identificador de usuario en una variable de session
        #request.session['userId'] =u.id
        #recuperar el identificador de usuario de una variable de sesion
        #id = request.session['userId']
        #recordad que para usar las variable de session hace falta
        #activas las cookies como se muestra en el script test_server.py
        #comprobar si variable 'counter' esta guardada en session
        #if 'counter' in request.session:
        return self.create_response(request, statsDict)

    def cat_move(self, request, args):

        id = request.session['gameId']
        games = Game.objects.filter(id=id)
        statsDict = {}        
        game = games[0]
        uclick = request.POST['click']
        click = int(uclick) 

        try:
            if 'select' not in request.session: #Si no existe la creamos
                request.session['select'] = 0

            if (request.session['select'] == 0): #Comprueba pos.inicial

                request.session['select'] = 1 #Cambiamos para que la siguiente llamada se meta en pos.final

                print("ORGIGEN", click)

                flag = 0
                
                #Comprobamos si existe algun gato en esta posicion
               
                if (game.cat1 == click):
                    request.session['catnumber'] = 1
                    flag = 1
                elif (game.cat2 == click):
                    request.session['catnumber'] = 2
                    flag = 1
                elif (game.cat3 == click):
                    request.session['catnumber'] = 3
                    flag = 1
                elif (game.cat4 == click):
                    request.session['catnumber'] = 4
                    flag = 1

                if (flag == 0):

                    statsDict['message'] = "Error en pos inicial"
                    statsDict['value'] = -1
                    statsDict['result'] = False 

                else:
                    cm = Move(origin=click, game=game)
                    cm.save()                    
                    request.session['moveId'] = int(cm.id) #Guardamos la id
                    statsDict['message'] = "La posicion es valida"
                    statsDict['value'] = cm.id
                    statsDict['result'] = True 
                    request.session['origin'] = click #Guardamos la posicion de origen para usarla en la siguiente llamada


                return self.create_response(request, statsDict)

            elif (request.session['select'] == 1): #Comprueba pos. final

                request.session['select'] = 0
                moveId = int(request.session['moveId'])
                cms = Move.objects.filter(id=moveId)
                cm = cms[0]
                cm.target = click
                cm.save()

                print("DESTINO", request.session['origin'], click)

                if (UserResource.isValidCatMove(self, game, request.session['origin'], click) == True):
                    if (request.session['catnumber'] == 1):
                        game.cat1 = click
                    elif (request.session['catnumber'] == 2):
                        game.cat2 = click                    
                    elif (request.session['catnumber'] == 3):
                        game.cat3 = click
                    elif (request.session['catnumber'] == 4):
                        game.cat4 = click
                    game.catTurn = -1
                    request.session['amIcat'] = 1
                    game.save()

                    statsDict['message'] = "La posicion es valida"
                    statsDict['value'] = cm.id
                    statsDict['result'] = True 
                
                else:
                    statsDict['message'] = "La posicion final no es valida"
                    statsDict['value'] = -1
                    statsDict['result'] = False 

                    game.catTurn = 1
                    game.save()



        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False
        return self.create_response(request, statsDict)


    def mouse_move(self, request, args):
     
        id = request.session['gameId']
        games = Game.objects.filter(id=id)
        statsDict = {}        
        game = games[0]
        uclick = request.POST['click']
        click = int(uclick) 

        try:

                if (UserResource.isValidMouseMove(self, game,  game.mouse, click) == True):

                    cm = Move(origin=game.mouse, target=click, game=game)
                    cm.save()
                    
                    game.mouse = click                 

                    statsDict['message'] = "La posicion es valida"
                    statsDict['value'] = cm.id
                    statsDict['result'] = True 

                    game.catTurn = 1
                    request.session['amIcat'] = -1
                    game.save()
                
                else:
                    statsDict['message'] = "La posicion final no es valida"
                    statsDict['value'] = -1
                    statsDict['result'] = False 

        except Exception as e:
            #en caso de error reportalo
            statsDict['message'] = e.message
            statsDict['value'] = -1
            statsDict['result'] = False
        return self.create_response(request, statsDict)

        
    def move(self, request, *args, **kwargs):
        
        statsDict = {}

        gameid = request.session['gameId']
        games = Game.objects.filter(id=gameid)
        game = games[0]

        if 'amIcat' not in request.session:
            request.session['amIcat'] = 1

        if (game.catTurn == 1):
            return UserResource.cat_move(self, request, args)
        elif (game.catTurn == -1):
            return UserResource.mouse_move(self, request, args)
        else:
            statsDict['value'] = -1
            statsDict['result'] = False
            statsDict['message'] = "Error al mover"
            return self.create_response(request, statsDict)

    def status(self, request, *args, **kwargs):
        turn = request.POST['type']
        request.session['cookie'] = request.POST['cookie']
        statsDict = {}

        if 'gameId' in request.session:
            gameid = request.session['gameId']
            games = Game.objects.filter(id=gameid)
            game = games[0]

            if (turn == "mouse"):
                    statsDict['value'] = 2**(game.mouse)
                    statsDict['result'] = True
                    statsDict['message'] = "La posicion del raton"
            elif (turn == "cats"):
                    statsDict['value'] = ((2**(game.cat1)) | (2**(game.cat2)) | (2**(game.cat3)) | (2**(game.cat4)))
                    statsDict['result'] = True
                    statsDict['message'] = "La posicion del gato"
            elif (turn == "myTurn"):


                    if (request.session['amIcat'] == 1):
                        statsDict['value'] = game.catTurn
                        statsDict['result'] = True
                        statsDict['message'] = "-1-> not my turm , 1-> myturn"

                    else:
                        statsDict['value'] = - game.catTurn
                        statsDict['result'] = True
                        statsDict['message'] = "-1-> not my turm , 1-> myturn"                        
            else:
                    statsDict['value'] = -1
                    statsDict['result'] = False
                    statsDict['message'] = "Argumento invalido."                

            return self.create_response(request, statsDict)
        statsDict['value'] = 0
        statsDict['result'] = False
        statsDict['message'] = "No hay juego en la session"
        return self.create_response(request, statsDict)


    def isValidCatMove(self, game,  origin, target):
    #bitwise comparison,

        if (game.cat1 == target or
            game.cat2 == target or
            game.cat3 == target or
            game.cat4 == target or
            game.mouse == target):
              return False
        if not (game.cat1 == origin or
                game.cat2 == origin or
                game.cat3 == origin or
                game.cat4 == origin ):
            return False

        if not ((origin-7)==target or (origin-9)==target):
            return False
        #must be your turn
        if game.catTurn == 0:
                return False

        return True

    def isValidMouseMove(self, game,  origin, target):
    #bitwise comparison,
        if (game.cat1 == target or
            game.cat2 == target or
            game.cat3 == target or
            game.cat4 == target or
            game.mouse == target):
              return False
        if not (game.mouse == origin):
            return False

        if not ((origin-7)==target or (origin-9)==target or (origin+7)==target or (origin+9)==target):
            return False
        #must be your turn
        if game.catTurn == 1:
                return False

        return True
