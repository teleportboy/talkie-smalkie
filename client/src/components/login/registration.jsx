import React from "react";

export function Registration() {
  const onSubmit = (event) => {
    event.preventDefault();
    
    const formData = new FormData(event.target.parentNode);
    const formProps = Object.fromEntries(formData);  

    fetch('/registration', {
      method: 'POST',
      headers: {
        'Accept': 'application/json',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(formProps)
    }).then(response => {
      return response.json();
    }).then(data => {
      console.log(data);
    });
  }

  return (
    <form className="login">
      <div className="username-input item">
        <label htmlFor="uname"><b>Username</b></label>
        <input id="uname_reg" type="text" placeholder="Enter Username" name="uname" required />
      </div>
      <div className="password-input item">
        <label htmlFor="psw"><b>Password</b></label>
        <input id="psw_reg" type="password" placeholder="Enter Password" name="psw" required />
      </div>
      
      <button onClick={onSubmit} type="submit">Create</button>
    </form>
  );
}