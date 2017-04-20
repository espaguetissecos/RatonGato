from django.core.validators import MaxValueValidator, MinValueValidator
from django.db import models

class User(models.Model):

    username = models.CharField(max_length=128, unique=True)
    password = models.CharField(max_length=128)

    def __unicode__(self):  #For Python 2, use __str__ on Python 3
        return self.username


class Game(models.Model):
    catUser = models.ForeignKey(User, related_name= 'gamecatUsers')
    mouseUser = models.ForeignKey(User, related_name= 'gamemouseUsers', null=True)
    cat1 = models.IntegerField(default=57, null=False)
    cat2 = models.IntegerField(default=59, null=False)
    cat3 = models.IntegerField(default=61, null=False)
    cat4 = models.IntegerField(default=63, null=False)
    mouse = models.IntegerField(default=4, null=False)
    catTurn = models.IntegerField(default=1, null=False)

    def __unicode__(self):      #For Python 2, use __str__ on Python 3
        return "Game: " + self.mouseUser.username + " vs " + self.catUser.username

class Move(models.Model):
    origin = models.IntegerField(null=False, validators=[MinValueValidator(0), MaxValueValidator(63)])
    target = models.IntegerField(null=True, blank=True, validators=[MinValueValidator(0), MaxValueValidator(63)])
    game = models.ForeignKey(Game)

    def __unicode__(self):  #For Python 2, use __str__ on Python 3
        return self.game
