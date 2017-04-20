# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0002_auto_20151006_1243'),
    ]

    operations = [
        migrations.AlterField(
            model_name='game',
            name='mouseUser',
            field=models.ForeignKey(related_name='gamemouseUsers', to='server.User', null=True),
        ),
    ]
