from django import forms
from server.models import User

class AddUserForm(forms.ModelForm):
    username = forms.CharField(max_length=128, help_text="Please enter your name.")
    password = forms.CharField(max_length=128, widget=forms.PasswordInput(), help_text="Please enter the password.")
    password_2 = forms.CharField(max_length=128, widget=forms.PasswordInput(),  help_text="Please confirm the password.")


    # An inline class to provide additional information on the form.
    class Meta:
        # Provide an association between the ModelForm and a model
        model = User
        fields = ('username','password')


class LoginForm(forms.ModelForm):
    username = forms.CharField(max_length=128, help_text="Please enter your name.")
    password = forms.CharField(max_length=128, widget=forms.PasswordInput(), help_text="Please enter the password.")

    class Meta:
        # Provide an association between the ModelForm and a model
        model = User
        fields = ('username', 'password')
