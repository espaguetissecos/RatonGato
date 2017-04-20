# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.core.validators


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Game',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('cat1', models.IntegerField(default=57)),
                ('cat2', models.IntegerField(default=59)),
                ('cat3', models.IntegerField(default=61)),
                ('cat4', models.IntegerField(default=63)),
                ('mouse', models.IntegerField(default=4)),
                ('catTurn', models.IntegerField(default=1)),
            ],
        ),
        migrations.CreateModel(
            name='Move',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('origin', models.IntegerField(default=4, validators=[django.core.validators.MinValueValidator(0), django.core.validators.MaxValueValidator(63)])),
                ('target', models.IntegerField(default=4, validators=[django.core.validators.MinValueValidator(0), django.core.validators.MaxValueValidator(63)])),
                ('game', models.ForeignKey(to='server.Game')),
            ],
        ),
        migrations.CreateModel(
            name='User',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('username', models.CharField(max_length=128)),
                ('password', models.CharField(max_length=128)),
            ],
        ),
        migrations.AddField(
            model_name='game',
            name='catUser',
            field=models.ForeignKey(related_name='gamecatUsers', to='server.User'),
        ),
        migrations.AddField(
            model_name='game',
            name='mouseUser',
            field=models.ForeignKey(related_name='gamemouseUsers', to='server.User'),
        ),
    ]
