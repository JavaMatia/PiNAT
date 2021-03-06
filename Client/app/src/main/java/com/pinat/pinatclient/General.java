package com.pinat.pinatclient;

import android.os.Bundle;

import com.google.android.material.tabs.TabLayout;

import androidx.viewpager.widget.ViewPager;
import androidx.appcompat.app.AppCompatActivity;

import com.pinat.pinatclient.Adapters.SectionsPagerAdapter;

public class General extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_general);

        Bundle params = getIntent().getExtras();
        if (params == null) return;
        String response = params.getString("response"); // Get response, pass to the adapter

        SectionsPagerAdapter sectionsPagerAdapter = new SectionsPagerAdapter(this,
                getSupportFragmentManager(), response);
        ViewPager viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(sectionsPagerAdapter);
        TabLayout tabs = findViewById(R.id.tabs);
        tabs.setupWithViewPager(viewPager);


    }
}