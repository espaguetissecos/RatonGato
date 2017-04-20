# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.core.validators


class Migration(migrations.Migration):

    dependencies = [
        ('server', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='move',
            name='origin',
            field=models.IntegerField(validators=[django.core.validators.MinValueValidator(0), django.core.validators.MaxValueValidator(63)]),
        ),
        migrations.AlterField(
            model_name='move',
            name='target',
            field=models.IntegerField(validators=[django.core.validators.MinValueValidator(0), django.core.validators.MaxValueValidator(63)]),
        ),
    ]
