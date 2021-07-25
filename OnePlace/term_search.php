<?php
session_start();

if(isset($_POST['submit'])){
    echo filter_input(INPUT_POST, 'terms');
}

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

