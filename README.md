
<!-- PROJECT LOGO -->
<br />
<div align="left">

  <h1 align="center">Web App Client using REST API</h1>

<div align="center">
 <a href="https://github.com/alexk1923/Football-Data-Remake-Public">
  <img src="https://madooei.github.io/cs421_sp20_homepage/assets/client-server-1.png" alt="Logo" width="240" height="80">
  </a>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

The server expose a REST API, simulating an online library.

The client send different requests to the server based on user input. Its purpose
is to offer an interface for the virtual library.

**More**: <a href="/Tema3_2021-2022.pdf">Project Info [RO]</a>

- The app is based on more source file, organised by their functionality.
 - `client.c` - manage user input
 - `prompts.c` - provides a command line interface to show what data is needed
 to be entered next
 - `commands.c` - execute an action based on the command, make a server request
 and get the result
 - `helpers.c` - client-server connectivity functions
 - `requests.c` - server message constructors fucntions
 - `parson.c` - contains a library that works with strings in JSON format
 - `buffer.c` - helpers functions to work with buffers, used in `helpers.c`
 
 
 
 - The client can make multiple requests to the server, the result will be
 returned as successful or error. Actions that the user can initiate are:
    - register
    - login
    - change own library content
    - get information about the books stored in the library
    - logout
    - exit the app
    
## Functionality

- The app is waiting for the user to type a command. If valid, a prompt with
required fields will be displayed. Otherwise, a help list with possible commands
will be shown in the console instead.
- The response from the server will be evaluated in **get_response_status**,
printing a message to the console regarding the status.
- Possible commands:
    - *register*: create new account. (requirements: the chosen username doesn't
    already exist in the server database)
    - *login* : returns a cookie for the current session
    - *enter_library* : returns the JWT to grant access further
    - *get_books* : returns some basic information about user's books
    - *get_book* : reutnrs detailed information for specific book, by id
    - *add_book* : add new book in the library
    - *delete_book* : remove a book by id
    - *logout*: reset the Authorization Token and Connection Cookie
    - *exit*: free all resources and stops application

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* [![C][C]][C-url]


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started


### How to run the app


Clone the repo
   ```sh
   git clone git@github.com:alexk1923/WebClient-REST-API.git
   ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

To run the app:

1. Use the Makefile to get the `client` file
   ```sh
   make
   ```
   
2. Run
```sh
./client
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Sources

<a href="https://ocw.cs.pub.ro/courses/pc/laboratoare/10 https://github.com/kgabis/parson">Parson Library</a>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
[Typescript]: https://img.shields.io/badge/TypeScript-007ACC?style=for-the-badge&logo=typescript&logoColor=white
[Typescript-url]: https://www.typescriptlang.org/
[Express]: https://img.shields.io/badge/Express.js-404D59?style=for-the-badge
[Express-url]: http://expressjs.com/
[SASS]:https://img.shields.io/badge/Sass-CC6699?style=for-the-badge&logo=sass&logoColor=white
[SASS-url]:https://sass-lang.com/
[C]:https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]:https://www.w3schools.com/c/c_intro.php
