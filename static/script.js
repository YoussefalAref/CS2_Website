if(!localStorage.token){
    // window.location.href="login.html"
}
function $(d){
    return document.querySelector($)
}
// User Suggestions
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        users = JSON.parse(this.responseText)
        users.forEach(element => {
            $("suggestions").innerHTML+= `
                <div class="sugesstion">
                    <span class="user">
                        <img style="height:100%;width:100%;" src="https://picsum.photos/200/300?random=3">
                        <!-- <i class="fa-solid fa-user"></i>  -->
                    </span>
                    <p>${element.name}</p>
                    <div>
                        <i class="fa-solid fa-user-plus"></i>
                    </div>
                </div>`
        });
    }
};
xhttp.open("GET", "filename", true);
xhttp.send();



// Publish posts

function Publish(d){

}
var xhttp = new XMLHttpRequest();
xhttp.open("GET", "filename", true);
xhttp.send();