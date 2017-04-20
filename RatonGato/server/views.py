from django.shortcuts import render
from server.forms import AddUserForm, LoginForm
from server.models import User
from django.http import HttpResponse

def index(request):
    #return HttpResponse("Rango says hey there world!")
    context_dict = {'boldmessage': ""}
    return render(request, 'RatonGato/index.html', context_dict)

def add_user(request):
    # A HTTP POST?
    if request.method == 'POST':
        form = AddUserForm(request.POST)

        # Have we been provided with a valid form?
        if (form.is_valid() and form.cleaned_data['password'] == form.cleaned_data['password_2']):
            # Save the new category to the database.
            form.save(commit=True)

            context_dict = {'boldmessage': "Usuario creado correctamente"}
            return render(request, 'RatonGato/index.html', context_dict)
        else:
            context_dict = {'boldmessage': "Nombre de usuario ya existe o contrasenyas no coinciden."}
            # The supplied form contained errors - just print them to the terminal.
            render(request,'RatonGato/add_user.html', context_dict)
    else:
        # If the request was not a POST, display the form to enter details.
        form = AddUserForm()

    # Bad form (or form details), no form supplied...
    # Render the form with error messages (if any).
    return render(request, 'RatonGato/add_user.html', {'form': form})

def login(request):
    # A HTTP POST?
    if request.method == 'POST':
        form = LoginForm(request.POST)

        # cleaned_data[]
        # Have we been provided with a valid form?
        if (form.is_valid() and User.objects.filter(username=form.cleaned_data['username'], password=form.cleaned_data['password']).exists()):
            # Save the new category to the database.
            context_dict = {'boldmessage': "Usuario logueado correctamente"}
            return render(request, 'RatonGato/index.html', context_dict)
        else:
            context_dict = {'boldmessage': "Nombre de usuario o contrasenya incorrecta."}
            # The supplied form contained errors - just print them to the terminal.
            render(request,'RatonGato/login.html', context_dict)
    else:
        # If the request was not a POST, display the form to enter details.
        form = LoginForm()

    # Bad form (or form details), no form supplied...
    # Render the form with error messages (if any).
    return render(request, 'RatonGato/login.html', {'form': form})

